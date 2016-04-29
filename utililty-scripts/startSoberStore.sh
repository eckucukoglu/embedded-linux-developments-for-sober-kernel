#!/bin/sh

uwsgi --socket 0.0.0.0:1050 --chdir /home/burakmert/Projects/MMIS/ApplicationServer/appServer --master --workers 1 --buffer-size=32768  --module=appServer.wsgi:application --env DJANGO_SETTINGS_MODULE=appServer.settings --pidfile=/tmp/appStore.pid --uid=1001 --gid=1001 --harakiri=20 --max-request=1000 --vacuum