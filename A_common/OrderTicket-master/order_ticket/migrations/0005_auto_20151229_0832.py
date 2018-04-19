# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('order_ticket', '0004_ticketorder'),
    ]

    operations = [
        migrations.AlterField(
            model_name='ticketorder',
            name='user',
            field=models.ForeignKey(to='mylogin.Passenger'),
        ),
    ]
