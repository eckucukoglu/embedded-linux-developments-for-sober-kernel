# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0019_auto_20160225_0841'),
    ]

    operations = [
        migrations.AlterField(
            model_name='application',
            name='size',
            field=models.DecimalField(default=1.6, max_digits=4, decimal_places=2),
        ),
    ]
