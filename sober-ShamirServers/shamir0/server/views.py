from django.shortcuts import render
from django.http import HttpResponse
from django.core.exceptions import ObjectDoesNotExist
from models import Secret,Client
import json
from django.views.decorators.csrf import csrf_exempt

@csrf_exempt
def processRequest(request):	
	response = {}
	response["isUpdated"] = 0
	response["error"] = 1
	response["message"] = "check parameters"
	
	# if request.method == 'GET':		
	# 	client_serial = request.GET.get("serial",None)
	# elif request.method == 'POST':
	# 	client_serial = request.POST.get("serial",None)		
	# 	client_share = request.POST.get("share",None)
	# try:
	# 	client = Client.objects.get(serial=client_serial)		
	# except ObjectDoesNotExist:
	# 		response["message"] = "Client does not exist"
	# 		response["error"] = 1
	# 		return HttpResponse(json.dumps(response))
	# if(client_serial):
	# 	client = Client.objects.get(serial=client_serial)		
	# 	if request.method == 'POST':
	# 		if(client_share):
	# 			client_share = Secret(share=client_share,client=client)			
	# 			client_share.save()		
	# 			response["isUpdated"] = 1	
	# 			response["message"] = "Share Updated"							
	# 	elif request.method == 'GET':
	# 		client_share = Secret.objects.get(client=client)
	# 		print "In get method"
	# 		print "Client serial is: "+client_serial
	# 		response["share"] = client_share.share
	# 		response["message"] = "Share sent"
	# 		print response


	if request.method == 'GET':
		client_serial = request.GET.get("serial",None)
		command = request.GET.get("command",None)
		if client_serial:
			try:
	 			client = Client.objects.get(serial=client_serial)		
	 		except ObjectDoesNotExist:
	 			response["message"] = "Client does not exist"
	 			response["error"] = 1
	 			return HttpResponse(json.dumps(response))
			if(command == "update"):
				client_share = request.GET.get("share",None)
				if(client_share):
					client_share = Secret(share=client_share,client=client)			
	 				client_share.save()		
	 				response["isUpdated"] = 1	
	 				response["message"] = "Share Updated"
	 				response["error"] = 0
			elif(command == "get"):
				client_share = Secret.objects.get(client=client)	 			
	 			response["share"] = client_share.share
	 			response["message"] = "Share sent"
	 			response["error"] = 0	 					
	return HttpResponse(json.dumps(response))
		
		
		