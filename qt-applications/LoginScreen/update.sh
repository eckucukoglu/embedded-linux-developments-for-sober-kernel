#!/bin/bash

paxctl -C LoginScreen
paxctl -pemxr LoginScreen
cp -f LoginScreen /media/arcelik/ROOTFS/usr/bin/.
echo "LoginScreen copied in /media/arcelik/ROOTFS/usr/bin/" 

