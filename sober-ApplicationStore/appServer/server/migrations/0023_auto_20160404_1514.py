# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0022_auto_20160404_1510'),
    ]

    operations = [
        migrations.AlterField(
            model_name='developer',
            name='developer_name',
            field=models.CharField(max_length=400),
        ),
        migrations.AlterField(
            model_name='developer',
            name='filePath',
            field=models.CharField(max_length=400),
        ),
    ]
