# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import datetime
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('order_ticket', '0005_auto_20151229_0832'),
    ]

    operations = [
        migrations.AddField(
            model_name='ticketorder',
            name='order_time',
            field=models.DateTimeField(default=datetime.datetime(2015, 12, 29, 10, 8, 30, 911932, tzinfo=utc)),
            preserve_default=False,
        ),
    ]
