# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import datetime
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0008_auto_20160202_0759'),
    ]

    operations = [
        migrations.AddField(
            model_name='application',
            name='filePath',
            field=models.CharField(default=datetime.datetime(2016, 2, 2, 13, 0, 10, 656941, tzinfo=utc), max_length=200),
            preserve_default=False,
        ),
        migrations.AlterField(
            model_name='application',
            name='packageFile',
            field=models.FileField(upload_to=b'applications/'),
        ),
    ]
