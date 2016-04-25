#!/bin/bash

mv Appstore Soberstore

paxctl -C Soberstore
paxctl -pemxr Soberstore
cp -f Soberstore /media/arcelik/ROOTFS/usr/bin/.

sync
