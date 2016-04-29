#!/bin/sh


#openssl dgst -sha256 -sign Certificates/Dev1/dev1.key.pem -out apps/CpuAndMemoryMonitor.sign apps/CpuAndMemoryMonitor

appFile=$1
append='App'
signAppend='.sign'
signFile=$appFile$signAppend
appFolder=$1$append
tarAppend='.tar.gz'
tarFile=$appFolder$tarAppend
certificateDir='/home/burakmert/Projects/MMIS/scripts/Certificates'

mkdir $appFolder
mv $appFile $appFolder
openssl dgst -sha256 -sign $certificateDir/Dev1/dev1.key.pem -out $appFolder/$signFile $appFolder/$appFile
tar -zcvf $tarFile $appFolder