# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0020_auto_20160225_0848'),
    ]

    operations = [
        migrations.AddField(
            model_name='application',
            name='cgroup',
            field=models.CharField(default=b'none', max_length=30, choices=[(b'memlimited0', b'memlimited0'), (b'cpulimited0', b'cpulimited0'), (b'cpulimited1', b'cpulimited1'), (b'none', b'none')]),
        ),
    ]
