# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import datetime
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0011_auto_20160202_1457'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='application',
            name='icon',
        ),
        migrations.AddField(
            model_name='application',
            name='iconFile',
            field=models.FileField(default=datetime.datetime(2016, 2, 2, 15, 15, 12, 147426, tzinfo=utc), upload_to=b'application/icons/'),
            preserve_default=False,
        ),
    ]
