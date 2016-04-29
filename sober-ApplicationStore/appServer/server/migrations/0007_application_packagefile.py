# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import datetime
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0006_auto_20160201_1441'),
    ]

    operations = [
        migrations.AddField(
            model_name='application',
            name='packageFile',
            field=models.FileField(default=datetime.datetime(2016, 2, 1, 15, 49, 31, 425416, tzinfo=utc), upload_to=b'applications/%Y/%m/%d'),
            preserve_default=False,
        ),
    ]
