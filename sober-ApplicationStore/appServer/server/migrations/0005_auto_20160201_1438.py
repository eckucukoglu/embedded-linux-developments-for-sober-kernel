# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import datetime
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0004_auto_20160201_1438'),
    ]

    operations = [
        migrations.AlterField(
            model_name='application',
            name='pub_date',
            field=models.DateTimeField(default=datetime.datetime(2016, 2, 1, 14, 38, 59, 87130, tzinfo=utc), verbose_name=b'date_published'),
        ),
    ]
