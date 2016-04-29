# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0002_secret_singleton_instance'),
    ]

    operations = [
        migrations.CreateModel(
            name='Client',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('serial', models.CharField(max_length=200)),
            ],
        ),
        migrations.RemoveField(
            model_name='secret',
            name='id',
        ),
        migrations.RemoveField(
            model_name='secret',
            name='singleton_instance',
        ),
        migrations.AddField(
            model_name='secret',
            name='client',
            field=models.OneToOneField(primary_key=True, default=None, serialize=False, to='server.Client'),
        ),
    ]
