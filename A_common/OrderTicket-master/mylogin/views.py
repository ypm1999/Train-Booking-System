# coding=UTF-8
from django.shortcuts import render
from django.core.urlresolvers import reverse
from django.contrib import auth
from django import forms
from django.contrib.auth.models import User
from django.contrib.auth.forms import UserCreationForm, AuthenticationForm
from django.views.decorators.csrf import csrf_exempt
from django.http import *
from mylogin.models import Passenger

class RegisterForm(forms.Form):
	username = forms.CharField(max_length=10, label='用户名', required=True, 
		error_messages={'required': '请输入您的用户名'})
	email = forms.EmailField(label='邮箱', required=True)
        name = forms.CharField(max_length=10,label='真实姓名',required=True)
	identity = forms.CharField(max_length=18, min_length=17, label='身份证', required=True)
	password1 = forms.CharField(label='密码', widget=forms.PasswordInput)
	password2 = forms.CharField(label='确认密码', widget=forms.PasswordInput)
	
	def clean_identity(self):
		identity = self.cleaned_data['identity']
		if not identity.encode('ascii').startswith('130'):
			raise forms.ValidationError("Not start with 130!")
		return identity

	def clean(self):
		pass1 = self.cleaned_data['password1']
		pass2 = self.cleaned_data['password2']
		if pass1 != pass2:
			del self.cleaned_data['password1']
			raise form.ValidationError("password not match!")
		return self.cleaned_data


def index(request):
	return render(request, "mylogin/index.html", {})


def register(request):
	if request.method == 'POST':
		form = RegisterForm(request.POST)
		if form.is_valid():
			user = User.objects.create_user(username=form.cleaned_data['username'], 
				    password=form.cleaned_data['password1'], 
				    email=form.cleaned_data['email'])
			user.save
			passenger = Passenger(user=user, name=form.cleaned_data['name'],identity=form.cleaned_data['identity'])
			passenger.save()
			print user.passenger.identity
			user = auth.authenticate(username=request.POST['username'], password=request.POST['password1'])
			auth.login(request, user)
			return HttpResponseRedirect(reverse('mylogin:profile'))
	else:
		form = RegisterForm()
	return render(request, "mylogin/register.html", {'form': form,})


def profile(request):
	if not request.user.is_authenticated():
		return HttpResponseRedirect(reverse('mylogin:index'))
	return render(request, "mylogin/profile.html", {'user': request.user,})

def mytickets(request):
	if not request.user.is_authenticated():
		return HttpResponseRedirect(reverse('mylogin:login'))
        passenger = request.user.passenger
        tickets = []
        for i in passenger.ticketorder_set.all().order_by("-order_time"):
            tickets.append(i.getTicketInfo())
	return render(request, "mylogin/mytickets.html", {'tickets':tickets})


def login(request):
	if request.method == 'POST':
		form = AuthenticationForm(data=request.POST)
		if form.is_valid():
			user = auth.authenticate(username=request.POST['username'], password=request.POST['password'])
			auth.login(request, user)
			return HttpResponseRedirect(reverse('mylogin:profile'))
	else:
		form = AuthenticationForm()
	return render(request, "mylogin/login.html", {'form': form,})


def logout(request):
	auth.logout(request)
	# Redirect to a success page.
	return HttpResponseRedirect(reverse('order_ticket:index'))
