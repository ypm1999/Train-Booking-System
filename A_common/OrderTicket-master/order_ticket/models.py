# -*- coding:utf-8 -*- 
from django.db import models
from django.contrib.auth.models import User
from django.utils import timezone
from mylogin.models import *


def Float_to_Desc(f):
    h = int(f) 
    f = f - h
    s = str(int(f*60)) + "分"
    if h != 0:
        s = str(int(h)) + "小时" + s
    return s

class Train(models.Model):
    name = models.CharField(max_length=30)
    st_time = models.TimeField()
    ed_time = models.TimeField()
    seat = models.IntegerField()
    cost_hours = models.FloatField()
    frequency_days = models.IntegerField()

    @staticmethod
    def testNameIn(name):
        result = Train.objects.filter(name=name)
        if len(result) == 0:
            return None
        else:
            return result[0]

    def testContain(self,start,end):
        flag = False
        found = {"train":self.name}
        cost = 0
        for i in self.line_set.all():
            if i.station.name.find(start) != -1: 
                flag = True
                found["start"] = i.station.name
                found["st_order"] = i.order
                found["st_time"] = i.time
                cost = i.cost 
                continue
            if i.station.name.find(end) != -1:
                if flag:
                    found["end"] = i.station.name
                    found["ed_order"] = i.order
                    found["ed_time"] = i.time
                    found["cost"] = i.cost - cost
                    return found
        return None
                
    def getDescribe(self):
        desc = {}
        desc["name"] = self.name
        desc["st_time"] = self.st_time
        desc["ed_time"] = self.ed_time
        desc["seat"] = self.seat
        desc["cost_hours"] = Float_to_Desc(self.cost_hours)
        desc["frequency_days"] = self.frequency_days 
        desc["line"] = []
        lines = Line.objects.filter(train = self).order_by("order")  
        for i in lines:
            s = {}
            s["order"] = i.order
            s["station"] = i.station.name
            s["cost"] = i.cost 
            s["time"] = i.time
            desc["line"].append(s)
        desc["from"] = lines[0].station.name
        desc["to"] = lines[len(lines)-1].station.name
        desc["cost"] = lines[len(lines)-1].cost
        return desc

    def __unicode__(self):
        return self.name
    class Meta:
        ordering = ['st_time']

class Station(models.Model):
    name = models.CharField(max_length=30)

    @staticmethod
    def isNameLikeIn(name):
        result = Station.objects.filter(name__contains=name)
        if len(result) == 0:
            return False
        else:
            return True

    def __unicode__(self):
        return self.name
    class Meta:
        ordering = ['name']

class Line(models.Model):
    train = models.ForeignKey(Train)
    order = models.IntegerField()
    station = models.ForeignKey(Station)
    cost = models.FloatField()
    time = models.TimeField()
    def __unicode__(self):
        return u'%s %d %s %s'%(self.train,self.order,self.station,self.time)
    class Meta:
        ordering = ['order']

class TicketOrder(models.Model):
    order_time = models.DateTimeField()
    user = models.ForeignKey(Passenger)
    train = models.ForeignKey(Train)
    date = models.DateField()
    st = models.CharField(max_length=30)
    ed = models.CharField(max_length=30)
    cost = models.FloatField()
    @staticmethod
    def newTicketOrder(info,user):
        train = Train.testNameIn(info["train"])
        if train != None:
            p = TicketOrder(order_time = timezone.now(),user = user,train = train,date=info["date"],st=info["from"],ed=info["to"],cost=info["cost"]) 
            p.save()
            return True
        else:
            return False
    def getTicketInfo(self):
        ret = {}
        ret["order_time"] = self.order_time
        ret["name"] = self.user.name
        ret["identity"] = self.user.identity
        ret["train"] = self.train.name
        ret["date"] = self.date
        ret["st"] = self.st 
        ret["ed"] = self.ed 
        ret["cost"] = self.cost
        return ret

class TicketPool(models.Model):
    train = models.ForeignKey(Train)
    date = models.DateField()
    st_order = models.IntegerField()
    ed_order = models.IntegerField()
    number = models.IntegerField()
    def getInfo(self,st,ed):
        info = {}
        info["train"] = self.train.name
        info["date"] = self.date
        line = self.train.line_set.all()
        st -= 1
        ed -= 1
        info["from"] = line[st].station.name
        info["to"] = line[ed].station.name
        info["st_time"] = line[st].time 
        info["ed_time"] = line[ed].time 
        info["cost"] = line[ed].cost - line[st].cost 
        return info

    @staticmethod
    def findRemain(name,date,st_order,ed_order):
        train = Train.objects.filter(name=name)[0]
        items = TicketPool.objects.filter(train=train,date=date)
        tot = 0 
        for i in items:
            if i.st_order<= st_order and i.ed_order >= ed_order:
                tot += i.number 
        return tot

    @staticmethod
    def findOne(name,date,st_order,ed_order):
        train = Train.objects.filter(name=name)[0]
        items = TicketPool.objects.filter(train=train,date=date)
        for i in items:
            if i.st_order<= st_order and i.ed_order >= ed_order:
                return i.id 
        return -1

    def insert(self,st,ed):
        if st == ed:
            return
        result = TicketPool.objects.filter(train=self.train,date=self.date,st_order=st,ed_order=ed) 
        if len(result) == 0:
            TicketPool(train=self.train,date=self.date,st_order=st,ed_order=ed,number=1).save()
        else:
            result[0].number += 1
            result[0].save()
        return 

    def order(self,st,ed):
        if self.st_order <= st and self.ed_order >= ed and self.number >= 1:
                self.number -= 1
                self.save()
                self.insert(ed, self.ed_order)
                self.insert(self.st_order, st)
                return True
        else:
            return False 
    def __unicode__(self):
        return u'%s %s %d %d %d' %(self.train.name,self.date,self.st_order,self.ed_order,self.number)
# Create your models here.
