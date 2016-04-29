# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import datetime
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0002_application_icon'),
    ]

    operations = [
        migrations.AddField(
            model_name='application',
            name='download_url',
            field=models.CharField(default=datetime.datetime(2016, 2, 1, 14, 37, 3, 192048, tzinfo=utc), max_length=200),
            preserve_default=False,
        ),
    ]
