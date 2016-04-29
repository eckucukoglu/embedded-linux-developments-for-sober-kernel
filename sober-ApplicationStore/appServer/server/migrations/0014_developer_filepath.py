# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import datetime
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0013_auto_20160204_1146'),
    ]

    operations = [
        migrations.AddField(
            model_name='developer',
            name='filePath',
            field=models.CharField(default=datetime.datetime(2016, 2, 4, 11, 53, 43, 598198, tzinfo=utc), max_length=200),
            preserve_default=False,
        ),
    ]
