# coding=UTF-8
from __future__ import unicode_literals

from django.db import models
from django.contrib.auth.models import User

# Create your models here.

class Passenger(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    identity = models.CharField( max_length=18)
    name = models.CharField(max_length=10)
    def __unicode__(self):
        return u'%s %s' % (self.name,self.user.username)
