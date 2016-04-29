from django.contrib import admin

# Register your models here.

from .models import Client, Secret

admin.site.register(Client)
admin.site.register(Secret)