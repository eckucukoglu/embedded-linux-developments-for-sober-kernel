# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import server.models


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0016_auto_20160208_1345'),
    ]

    operations = [
        migrations.AddField(
            model_name='application',
            name='size',
            field=models.DecimalField(default=1.6, max_digits=4, decimal_places=2),
        ),
        migrations.AlterField(
            model_name='application',
            name='packageFile',
            field=models.FileField(upload_to=server.models.upload_to_path),
        ),
        migrations.AlterField(
            model_name='developer',
            name='certificate',
            field=models.FileField(upload_to=server.models.upload_to_path),
        ),
    ]
