from django.contrib import admin
from order_ticket.models import *
 
admin.site.register(Train)
admin.site.register(Line)
admin.site.register(Station)
admin.site.register(TicketPool)
admin.site.register(TicketOrder)
# Register your models here.
