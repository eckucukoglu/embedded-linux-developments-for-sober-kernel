from django.shortcuts import render
from django.http import HttpResponse
from .models import Application, Developer
from django.core.servers.basehttp import FileWrapper
from .forms import ApplicationForm, DeveloperForm
from django.shortcuts import render_to_response
from django.template import RequestContext
from django.shortcuts import redirect
from django.contrib.auth.decorators import login_required
from django.contrib.auth import authenticate, login, logout
from django.contrib import messages
from django.core.files.storage import default_storage
from django.core.files.base import ContentFile
from django.core.exceptions import ObjectDoesNotExist
import json
import gzip
from django.conf import settings
import os 
import tarfile
import time
import fnmatch
import OpenSSL
import hashlib
import shutil
import collections
from OpenSSL import crypto
from appServer import root_cert, store
from Crypto import Signature, PublicKey
from M2Crypto import RSA, X509

def date_handler(obj):
    return obj.isoformat() if hasattr(obj, 'isoformat') else obj

def index(request):
	json_response = {}
	json_response['applications'] = []		
	apps = Application.objects.all()	
	for app in apps:
		orderedApp = collections.OrderedDict()
		orderedApp['id'] = app.id
		orderedApp['name'] = app.application_name
		orderedApp['developer'] = app.developer.developer_name
		orderedApp['icon'] = app.iconName
		orderedApp['size'] = float(app.size)
		orderedApp['cgroup'] = app.cgroup
		orderedApp['version'] = float(app.version)
		dt = app.pub_date
		app_timestamp = int(time.mktime(dt.timetuple()))
		orderedApp['date'] = app_timestamp
		json_response['applications'].append(orderedApp)		
	return HttpResponse(json.dumps(json_response))

def show(request, application_id):
	json_response = collections.OrderedDict()	
	app = Application.objects.get(id=application_id)
	json_response['id'] = app.id		
	json_response['name'] = app.application_name		
	json_response['developer'] = app.developer.developer_name	
	json_response['icon'] = app.iconName		
	json_response['size'] = float(app.size)
	json_response['cgroup'] = app.cgroup
	json_response['version'] = float(app.version)
	dt = app.pub_date
	app_timestamp = int(time.mktime(dt.timetuple()))
	json_response['date'] = app_timestamp
		

	return HttpResponse(json.dumps(json_response))

def download(request, application_id):
	json_response = {}
	try:
		app = Application.objects.get(id=application_id)
	except ObjectDoesNotExist:
		return HttpResponse("Object does not exist")		
	response = HttpResponse(app.packageFile, content_type='application/x-gzip')
	response['Content-Disposition'] = 'attachment; filename=%s.tar.gz' % app.application_name
	return response

@login_required(login_url ='/login')
def new(request):	
	form = ApplicationForm()
	developers = Developer.objects.all()
	cgroup_choices = ['memlimited0','cpulimited0','cpulimited1','none'] 
	return render_to_response(
		'application/new.html',
		{'form':form, 'developers':developers, 'cgroup_choices':cgroup_choices},
		context_instance=RequestContext(request)
		)
	

def create_signature(app, file_path):
	tarf = tarfile.open(file_path,'r:gz')
	suffix='sha256'
	for f in tarf:
		if f.name.endswith(suffix):
			app.signature = tarf.extractfile(f).read().decode("utf-8")
			print(tarf.extractfile(f).read())
	app.save()
	return True


def check_signature(temp_path, developer_cert):	
	tfile = tarfile.open(temp_path, 'r:gz')
	tempAppPath = os.path.join(settings.MEDIA_ROOT,'tempApp')
	tfile.extractall(tempAppPath)
	tfile.close()	
	for root,dirs, files in os.walk(tempAppPath):
		for item in fnmatch.filter(files,"*"):
			if item.endswith(".sign"):
				signatureFile  = item
			else:		
				binary_path = root +"/" +item		
				size = int(os.path.getsize(binary_path))/float(1024)
				print "Binary size is " + str(size)							
				appFile = item
	print root			
	if signatureFile is None or appFile is None:
		shutil.rmtree(tempAppPath)
		return False, 0	
	else:		
		signature = open(os.path.join(root,signatureFile),'rt').read()
		appData = open(os.path.join(root,appFile),'rt').read()
		data = hashlib.sha256(appData).digest()  
		dev_cert_file = open(developer_cert,'rt').read()
		dev_cert = X509.load_cert_string(dev_cert_file,X509.FORMAT_PEM)
		public_key = dev_cert.get_pubkey()
		public_key_rsa = public_key.get_rsa()
		shutil.rmtree(tempAppPath)	
	try :		
		return	public_key_rsa.verify(data,signature,algo='sha256'), size
	except Exception as e:
		print e
		return False, 0
		


@login_required(login_url ='/login')
def create(request):	
	if request.method == 'POST':
		form = ApplicationForm(request.POST, request.FILES)	
		if form.is_valid():
			appFile = request.FILES['packageFile']
			tempPath = default_storage.save('tmp', ContentFile(appFile.read()))
			path2Seek = os.path.join(settings.MEDIA_ROOT,'tmp')
			file_path = os.path.join(settings.MEDIA_ROOT,'applications', '%s' % appFile)
			dev = Developer.objects.get(developer_name = request.POST["developer"])	
			developer_cert = dev.filePath
			checked, appSize = check_signature(path2Seek,developer_cert)
			print "Binary size in kB is  %f:" % appSize
			if checked:
				default_storage.delete(tempPath)
				app_name = request.POST["name"]	
				icon_name = app_name + '.png'
				cgroup = request.POST["cgroup"]
				version = request.POST["version"]
				application = Application(application_name=app_name , developer=dev, packageFile=request.FILES['packageFile'],filePath=file_path, iconName =icon_name, size=appSize, cgroup=cgroup, version=version)
				application.save()
				messages.add_message(request,messages.INFO,'Application Saved: Signature verified')			
			else:
				print("Exception!!!")
				default_storage.delete(tempPath)
				messages.add_message(request,messages.INFO,'Application Signature verification failed!')
				return redirect('new')
	return redirect('server.views.show', application_id=application.id)

@login_required(login_url ='/login')
def new_developer(request):	
	form = DeveloperForm()
	renderDict= {}
	renderDict['form']=form	
	return render_to_response(
		'developer/new.html',
		renderDict,
		context_instance=RequestContext(request)
		)

# If this fails, exception is thrown
def check_certificate(certFile):
	try:		
		cert = certFile.read()
		developer_cert = OpenSSL.crypto.load_certificate(OpenSSL.crypto.FILETYPE_PEM,cert)
		store_ctx = OpenSSL.crypto.X509StoreContext(store,developer_cert)	
		store_ctx.verify_certificate()
		return True
	except Exception as e:
		print(e)
		return False


@login_required(login_url ='/login')
def create_developer(request):
	if request.method == 'POST':
		form = DeveloperForm(request.POST, request.FILES)	
		if form.is_valid():			
			certFile = request.FILES['certFile']
			dev_name = request.POST["name"]	
			file_path = os.path.join(settings.MEDIA_ROOT,'certificates','%s' % dev_name, '%s' % certFile)	
			if(check_certificate(certFile)):						
				dev = Developer(developer_name=request.POST["name"] ,certificate=certFile,filePath=file_path)
				dev.save()				
				messages.add_message(request,messages.INFO,'Developer saved succesfully')
				return redirect('new_developer')
			else:				
				messages.add_message(request,messages.INFO,'Please provide a valid developer certificate!')
				return redirect('new_developer')

def login_user(request):
    logout(request)
    username = password = ''
    if request.POST:
        username = request.POST['username']
        password = request.POST['password']
        user = authenticate(username=username, password=password)
        if user is not None:
            if user.is_active:
                login(request, user)
                return HttpResponseRedirect('/main/')
    return render_to_response('login.html', context_instance=RequestContext(request))



