from django.db import models
from django.utils import timezone
from django.conf import settings
from pprint import pprint
import os

def upload_to_path(instance,filename):	
	if(instance.__class__.__name__=='Developer'):
		return (os.path.join(settings.MEDIA_ROOT,'certificates', '%s' % instance.developer_name,filename))
	elif(instance.__class__.__name__=='Application'):
		return (os.path.join(settings.MEDIA_ROOT,'applications', '%s' % instance.application_name,filename))
	print(str(instance.__class__.__name__))
	

class Developer(models.Model):
	certificate = models.FileField(upload_to=upload_to_path)
	developer_name = models.CharField(max_length=200)
	filePath = models.CharField(max_length=200)


	

class Application(models.Model):
	application_name = models.CharField(max_length=200)	
	pub_date = models.DateTimeField('date_published',default=timezone.now)		
	developer = models.ForeignKey(Developer, on_delete=models.CASCADE)
	iconName = models.CharField(max_length=200)
	packageFile = models.FileField(upload_to=upload_to_path, max_length=400)
	filePath = models.CharField(max_length=200)
	size = models.DecimalField(max_digits=8, decimal_places=2, default=1.6)	
	version = models.DecimalField(max_digits=4, decimal_places=1, default=1.0)
	MEMORY_LIMITED_0 = 'memlimited0'
	CPU_LIMITED_0 = 'cpulimited0'
	CPU_LIMITED_1 = 'cpulimited1'
	NONE = 'none'
	CGROUP_CHOICES = (
		(MEMORY_LIMITED_0, 'memlimited0'),
		(CPU_LIMITED_0, 'cpulimited0'),
		(CPU_LIMITED_1, 'cpulimited1'),
		(NONE, 'none'),
	)
	cgroup = models.CharField(max_length=30, choices=CGROUP_CHOICES, default=NONE)
	def __str__(self):
		return self.application_name

	#def upload_to_path(instance):
	#	return os.path.join(settings.MEDIA_ROOT,'applications', '%s' % instance.application_name)



