# -*- coding:utf-8 -*- 
from order_ticket.models import *
def tofloat(s):
    hour = 0
    mini = 0
    l = s.split("小时") 
    if s.find("小时") != -1:
        hour = int(l[0])
        l = l[1:]
    l = l[0].replace("分","")
    mini = int(l)
    return hour + mini*1.0/60  
    
    
l = open("gt.txt").read().split("\n")
for i in l[:-1]:
    tr = i.split(" | ")
    tmp = tr[0].split(" ")
    name = tmp[0]
    cost_hours = tofloat(tmp[1])
    st_time = tmp[2]
    ed_time = tmp[3]
    st = Train.objects.filter(name=name)[0]
    print st
    for j in tr[1:-1]:
        j = j.split(" ")
        order = j[1]
        station = j[2]
        res = Station.objects.filter(name=station)
        if len(res) == 0:
            p = Station(name=station)
            p.save()
            res = Station.objects.filter(name=station)
            print res
        station = res[0]    
        time = j[3]
        cost = j[4]
        li = Line.objects.filter(train=st,order=int(order))
        li = li[0]
        if cmp(time,'终点站')==0:
           time = ed_time 
        li.time = time
        li.save()
        
    

