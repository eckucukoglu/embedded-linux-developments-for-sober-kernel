#!/bin/sh


if [ $1 = 0 ]; then
	echo "Starting shamir0 server"
	uwsgi --socket 0.0.0.0:7002 --chdir /home/burakmert/Projects/MMIS/ShamirServers/shamir0 --master --workers 1 --buffer-size=32768  --module=shamir0.wsgi:application --env DJANGO_SETTINGS_MODULE=shamir0.settings --pidfile=/tmp/shamirServer.pid --harakiri=20 --max-request=1000 --vacuum # shamir1 
elif [ $1 = 1 ]; then
	echo "Stating shamir1 server"
	uwsgi --socket 0.0.0.0:8002 --chdir /home/burakmert/Projects/MMIS/ShamirServers/shamir1 --master --workers 1 --buffer-size=32768  --module=shamir1.wsgi:application --env DJANGO_SETTINGS_MODULE=shamir1.settings --pidfile=/tmp/shamirServer.pid --harakiri=20 --max-request=1000 --vacuum # shamir2
elif [ $1 = 2 ]; then
	echo "Starting shamir2 server"
	uwsgi --socket 0.0.0.0:9002 --chdir /home/burakmert/Projects/MMIS/ShamirServers/shamir2 --master --workers 1 --buffer-size=32768  --module=shamir2.wsgi:application --env DJANGO_SETTINGS_MODULE=shamir2.settings --pidfile=/tmp/shamirServer.pid --harakiri=20 --max-request=1000 --vacuum # shamir3
else
	echo "Only 0,1,2 servers are available"
fi