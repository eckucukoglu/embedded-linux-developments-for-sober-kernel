# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0010_auto_20160202_1451'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='application',
            name='download_url',
        ),
        migrations.RemoveField(
            model_name='application',
            name='signature',
        ),
    ]
