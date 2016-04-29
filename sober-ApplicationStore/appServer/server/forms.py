from django import forms

class ApplicationForm(forms.Form):
	packageFile = forms.FileField(label='Select application package')

class DeveloperForm(forms.Form):
	certFile = forms.FileField(label='Select Certificate')	
