# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('mylogin', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='passenger',
            name='name',
            field=models.CharField(default='haha', max_length=10),
            preserve_default=False,
        ),
    ]
