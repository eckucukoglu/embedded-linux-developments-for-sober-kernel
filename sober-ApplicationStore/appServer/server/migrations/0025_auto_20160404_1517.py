# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0024_auto_20160404_1516'),
    ]

    operations = [
        migrations.AlterField(
            model_name='application',
            name='application_name',
            field=models.CharField(max_length=200),
        ),
        migrations.AlterField(
            model_name='application',
            name='filePath',
            field=models.CharField(max_length=200),
        ),
        migrations.AlterField(
            model_name='application',
            name='iconName',
            field=models.CharField(max_length=200),
        ),
        migrations.AlterField(
            model_name='developer',
            name='developer_name',
            field=models.CharField(max_length=200),
        ),
        migrations.AlterField(
            model_name='developer',
            name='filePath',
            field=models.CharField(max_length=200),
        ),
    ]
