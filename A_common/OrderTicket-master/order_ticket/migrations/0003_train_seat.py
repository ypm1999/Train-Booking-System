# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('order_ticket', '0002_auto_20151228_0510'),
    ]

    operations = [
        migrations.AddField(
            model_name='train',
            name='seat',
            field=models.IntegerField(default=30),
            preserve_default=False,
        ),
    ]
