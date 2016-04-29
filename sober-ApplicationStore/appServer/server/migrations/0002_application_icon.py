# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='application',
            name='icon',
            field=models.CharField(default='iconcan', max_length=200),
            preserve_default=False,
        ),
    ]
