#!/bin/sh

if [ -z ${1} ]
then
	echo "The usage of this script is: ${0} /path/to/disk/to/be/encrypted"
	echo "Before starting, make sure that you have created /mnt/EncryptedDisk"
else
	encryptedDisk=${1}	
	cryptsetup create newMapper ${encryptedDisk}	 	
	mkfs /dev/mapper/newMapper
	mount -t ext2 /dev/mapper/newMapper /mnt/EncryptedDisk			
	echo "Encryption Completed!"
	echo "You can now write to /mnt/EncryptedDisk to write your encrypted disk."
fi
