# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0007_application_packagefile'),
    ]

    operations = [
        migrations.AlterField(
            model_name='application',
            name='download_url',
            field=models.CharField(default=b'hebele', max_length=200),
        ),
        migrations.AlterField(
            model_name='application',
            name='icon',
            field=models.CharField(default=b'None', max_length=200),
        ),
    ]
