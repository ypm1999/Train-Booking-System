from django.conf.urls import url
from django.contrib.auth.views import login, logout

from mylogin import views 

app_name = 'mylogin'
urlpatterns = [
	url(r'^$', views.index, name='index'),
	url(r'^register/$', views.register, name='register'),
	url(r'^login/$', views.login, name='login'),
	url(r'^logout/$', views.logout, name='logout'),
	url(r'^profile/$', views.profile, name='profile'),
	url(r'^mytickets/$', views.mytickets, name='mytickets'),
]
