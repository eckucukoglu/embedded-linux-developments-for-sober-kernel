# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0005_auto_20160201_1438'),
    ]

    operations = [
        migrations.AlterField(
            model_name='application',
            name='pub_date',
            field=models.DateTimeField(default=django.utils.timezone.now, verbose_name=b'date_published'),
        ),
    ]
