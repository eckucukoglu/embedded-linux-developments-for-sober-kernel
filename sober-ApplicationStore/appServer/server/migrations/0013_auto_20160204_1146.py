# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0012_auto_20160202_1515'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='application',
            name='iconFile',
        ),
        migrations.AlterField(
            model_name='developer',
            name='certificate',
            field=models.FileField(upload_to=b'certificates'),
        ),
    ]
