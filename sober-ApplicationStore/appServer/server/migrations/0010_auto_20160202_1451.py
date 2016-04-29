# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0009_auto_20160202_1300'),
    ]

    operations = [
        migrations.AlterField(
            model_name='application',
            name='signature',
            field=models.CharField(max_length=2000),
        ),
    ]
