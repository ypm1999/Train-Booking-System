from django.conf.urls import url
from django.contrib.auth.views import login, logout

from order_ticket.views import *

app_name = 'order_ticket'
urlpatterns = [
	url(r'^$', index, name='index'),
	url(r'^train/(\w+)$', train, name='train'),
	url(r'^order/(\d+)/(\d+)/(\d+)/$', order, name='order'),
        url(r'^about/$',about,name='about'),
]
