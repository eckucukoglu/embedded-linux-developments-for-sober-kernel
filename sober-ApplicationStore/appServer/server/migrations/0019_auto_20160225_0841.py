# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0018_auto_20160225_0836'),
    ]

    operations = [
        migrations.AlterField(
            model_name='application',
            name='size',
            field=models.FloatField(default=1.6),
        ),
    ]
