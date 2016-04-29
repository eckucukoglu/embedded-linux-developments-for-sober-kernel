# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import datetime
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0014_developer_filepath'),
    ]

    operations = [
        migrations.AddField(
            model_name='application',
            name='iconName',
            field=models.CharField(default=datetime.datetime(2016, 2, 4, 12, 2, 38, 450116, tzinfo=utc), max_length=200),
            preserve_default=False,
        ),
    ]
