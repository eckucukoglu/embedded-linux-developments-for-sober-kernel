#!/bin/sh

if [ -z ${1} ] 
then 
	echo "Usage of this script is: ${0} /path/to/filesystem/to/decrypt"
else
	filesystem=${1}
	umount.ecryptfs $filesystem
fi









