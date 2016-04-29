# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0025_auto_20160404_1517'),
    ]

    operations = [
        migrations.AddField(
            model_name='application',
            name='version',
            field=models.DecimalField(default=1.0, max_digits=4, decimal_places=1),
        ),
    ]
