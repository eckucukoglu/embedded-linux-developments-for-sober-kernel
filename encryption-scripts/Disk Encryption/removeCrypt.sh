#!/bin/sh

umount /mnt/EncryptedDisk	
cryptsetup remove newMapper
# TODO: completed message should be shown if operation has done successfully.
echo "Remove operation is Completed!"
