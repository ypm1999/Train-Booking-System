# -*- coding:utf-8 -*- 
from order_ticket.models import Station
l = open("station.txt").read().split("、")
for i in l:
    if i != "":
        if Station.objects.filter(name=i) != []:
            p = Station(name=i)
            p.save()


