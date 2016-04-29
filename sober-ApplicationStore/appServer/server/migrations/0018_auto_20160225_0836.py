# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0017_auto_20160225_0815'),
    ]

    operations = [
        migrations.AlterField(
            model_name='application',
            name='application_name',
            field=models.CharField(max_length=400),
        ),
        migrations.AlterField(
            model_name='application',
            name='filePath',
            field=models.CharField(max_length=400),
        ),
        migrations.AlterField(
            model_name='application',
            name='iconName',
            field=models.CharField(max_length=400),
        ),
    ]
