# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import server.models


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0023_auto_20160404_1514'),
    ]

    operations = [
        migrations.AlterField(
            model_name='application',
            name='packageFile',
            field=models.FileField(max_length=400, upload_to=server.models.upload_to_path),
        ),
    ]
