# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Application',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('application_name', models.CharField(max_length=200)),
                ('pub_date', models.DateTimeField(verbose_name=b'date_published')),
                ('signature', models.CharField(max_length=400)),
            ],
        ),
        migrations.CreateModel(
            name='Developer',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('certificate', models.CharField(max_length=200)),
                ('developer_name', models.CharField(max_length=200)),
            ],
        ),
        migrations.AddField(
            model_name='application',
            name='developer',
            field=models.ForeignKey(to='server.Developer'),
        ),
    ]
