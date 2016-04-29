# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0003_auto_20160308_0904'),
    ]

    operations = [
        migrations.RenameField(
            model_name='secret',
            old_name='secret_slice',
            new_name='share',
        ),
    ]
