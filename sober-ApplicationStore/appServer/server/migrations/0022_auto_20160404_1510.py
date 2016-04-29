# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0021_application_cgroup'),
    ]

    operations = [
        migrations.AlterField(
            model_name='application',
            name='application_name',
            field=models.CharField(max_length=800),
        ),
        migrations.AlterField(
            model_name='application',
            name='filePath',
            field=models.CharField(max_length=800),
        ),
        migrations.AlterField(
            model_name='application',
            name='iconName',
            field=models.CharField(max_length=800),
        ),
    ]
