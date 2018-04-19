# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import datetime
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('order_ticket', '0001_initial'),
    ]

    operations = [
        migrations.AlterModelOptions(
            name='line',
            options={'ordering': ['order']},
        ),
        migrations.AlterModelOptions(
            name='station',
            options={'ordering': ['name']},
        ),
        migrations.AlterModelOptions(
            name='train',
            options={'ordering': ['st_time']},
        ),
        migrations.AddField(
            model_name='line',
            name='time',
            field=models.TimeField(default=datetime.datetime(2015, 12, 28, 5, 10, 55, 48751, tzinfo=utc)),
            preserve_default=False,
        ),
    ]
