# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0015_application_iconname'),
    ]

    operations = [
        migrations.AlterField(
            model_name='developer',
            name='certificate',
            field=models.FileField(upload_to=b'certificates/'),
        ),
    ]
