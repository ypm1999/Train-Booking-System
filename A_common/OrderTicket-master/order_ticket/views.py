 # -*- coding:utf-8 -*- 
from django.shortcuts import *
from django.core.urlresolvers import reverse
from django.http import HttpResponse
from order_ticket.models import *
from django import forms
# Create your views here.

class SearchForm(forms.Form):
    start = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control','placeholder':'北京'}))
    end = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control','placeholder':'上海'}))
    date= forms.DateField(widget=forms.TextInput(attrs={'class': 'form_datetime','readonly': True}))

def search(start,end,date):
    if not (Station.isNameLikeIn(start) and Station.isNameLikeIn(end)):
        return []
    trains = Train.objects.all().order_by("st_time")
    result = [] 
    for i in trains:
        ret = i.testContain(start,end)
        if ret != None :
            ret["rem"] = TicketPool.findRemain(ret["train"],date,ret["st_order"],ret["ed_order"])
            ret["id"] = TicketPool.findOne(ret["train"],date,ret["st_order"],ret["ed_order"])
            result.append(ret)
    return result

def about(request):
    return render_to_response('about.html',RequestContext(request,{}))

def order(request,id,st,ed):
    if request.user.is_authenticated():
        user = request.user
        tick = TicketPool.objects.get(id=id)
        st = int(st)
        ed = int(ed)
        info = tick.getInfo(st,ed)
        if request.method == 'POST':
            result = "success"
            flag = TicketOrder.newTicketOrder(info,request.user.passenger)  
            if flag == False:
               result = "error" 
            else:
                flag = tick.order(st,ed)
                if flag == False:
                    retult = "notenought"
            return render_to_response('order.html',RequestContext(request,{'result':result,'order':info,'user':user}))
        else: 
            return render_to_response('order.html',RequestContext(request,{'order':info}))
    else:
         return HttpResponseRedirect(reverse('mylogin:login'))

def index(request):
    if request.method == 'POST':
        form = SearchForm(request.POST)
        if form.is_valid():
            dic = form.cleaned_data
            start = dic["start"]
            end = dic["end"]
            date = dic["date"]
            result = search(start,end,date)
            return render_to_response('index.html',RequestContext(request,{'message':result==[],'result':result,'form':form}))
    else:
        form = SearchForm() 
    return render_to_response('index.html',RequestContext(request,{'form':form}))
    
def train(request,name):
    result = Train.testNameIn(name)
    if result == None:
        result = {}
    else:
        result = result.getDescribe()
    return render_to_response('train.html',{'train': result})
