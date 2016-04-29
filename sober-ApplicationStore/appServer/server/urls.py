from django.conf.urls import url
from django.conf import settings

from . import views

urlpatterns = [
    url(r'^soberstore/$', views.index, name='index'),    
    url(r'^soberstore/new$', views.new, name='new'),
    url(r'^soberstore/create$', views.create, name='create'),
    url(r'^soberstore/(?P<application_id>[0-9]+)/$', views.show, name='show'),
    url(r'^soberstore/(?P<application_id>[0-9]+)/download/$', views.download, name='download'),
    url(r'^soberstore/developer/new$', views.new_developer, name='new_developer'),
    url(r'^soberstore/developer/create$', views.create_developer, name='create_developer'),
] 
