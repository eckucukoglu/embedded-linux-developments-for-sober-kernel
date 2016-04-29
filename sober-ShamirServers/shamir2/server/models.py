from django.db import models


class Client(models.Model):
	serial = models.CharField(max_length=200)

class Secret(models.Model):
	share = models.CharField(max_length=512)
	client = models.OneToOneField(Client, on_delete=models.CASCADE, primary_key=True, default=None)

	