# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Line',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('order', models.IntegerField()),
                ('cost', models.FloatField()),
            ],
        ),
        migrations.CreateModel(
            name='Station',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('name', models.CharField(max_length=30)),
            ],
        ),
        migrations.CreateModel(
            name='TicketPool',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('date', models.DateField()),
                ('st_order', models.IntegerField()),
                ('ed_order', models.IntegerField()),
                ('number', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='Train',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('name', models.CharField(max_length=30)),
                ('st_time', models.TimeField()),
                ('ed_time', models.TimeField()),
                ('cost_hours', models.FloatField()),
                ('frequency_days', models.IntegerField()),
            ],
        ),
        migrations.AddField(
            model_name='ticketpool',
            name='train',
            field=models.ForeignKey(to='order_ticket.Train'),
        ),
        migrations.AddField(
            model_name='line',
            name='station',
            field=models.ForeignKey(to='order_ticket.Station'),
        ),
        migrations.AddField(
            model_name='line',
            name='train',
            field=models.ForeignKey(to='order_ticket.Train'),
        ),
    ]
