# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
from django.conf import settings


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ('order_ticket', '0003_train_seat'),
    ]

    operations = [
        migrations.CreateModel(
            name='TicketOrder',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('date', models.DateField()),
                ('st', models.CharField(max_length=30)),
                ('ed', models.CharField(max_length=30)),
                ('cost', models.FloatField()),
                ('train', models.ForeignKey(to='order_ticket.Train')),
                ('user', models.ForeignKey(to=settings.AUTH_USER_MODEL)),
            ],
        ),
    ]
