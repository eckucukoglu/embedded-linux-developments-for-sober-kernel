#! /bin/sh

VERSION="[filesystem-crypter v0.1]"

version () {
  echo "$VERSION"
  echo
  echo "This script encrypt the given filesystem"
  echo "with a given password and using ecryptfs."
  echo
  echo "Directory paths that contains space character is not supported yet."
  exit 0
}

usage () {
  echo "Usage: `basename $1` filesystem [OPTION]

Mandatory options:
  -p <Password>		Password to be used at encryption.

Optional options:
  -fne			Enable filename encryption.
  -help			Print this help message.
  -version		Print version information.
  
Example usage:
  ./`basename $1` /secret-files -p 12345 -fne"
  exit 1
}

# filename encryption is disabled by default.
ecryptfs_enable_filename_crypto=no

while [ $# -gt 0 ]; do
  case $1 in
    -help | -h)
      usage $0
      ;;
    -fne) ecryptfs_enable_filename_crypto=yes; shift;
      ;;
    -p) shift; password=$1; shift;
      ;;
    -version | -v) version $0
      ;;
    *) filesystem="${filesystem}${1}"; shift;
      ;;    
  esac
done

# Open help, if missing parameter exist.
test -z $password || test ! -d ${filesystem} && usage $0

# Add tokens into kernel keyring
printf "%s" "${password}" | ecryptfs-add-passphrase --fnek > tmp.txt

# Now get the signature from the output of the above command
ecryptfs_sig=`awk 'NR == 2' tmp.txt | awk '{print $6}' | sed 's/\[//g' | sed 's/\]//g'`

# Now perform the mount
if [ "$ecryptfs_enable_filename_crypto" = "yes" ]; then 
	ecryptfs_fnek_sig=`awk 'NR == 3' tmp.txt | awk '{print $6}' | sed 's/\[//g' | sed 's/\]//g'`
	mount.ecryptfs $filesystem $filesystem -o key=passphrase:passphrase_passwd=${password},no_sig_cache=yes,verbose=no,ecryptfs_sig=${ecryptfs_sig},ecryptfs_fnek_sig=${ecryptfs_fnek_sig},ecryptfs_cipher=aes,ecryptfs_key_bytes=16,ecryptfs_passthrough=no,ecryptfs_enable_filename_crypto=${ecryptfs_enable_filename_crypto}
else
	mount.ecryptfs $filesystem $filesystem -o key=passphrase:passphrase_passwd=${password},no_sig_cache=yes,verbose=no,ecryptfs_sig=${ecryptfs_sig},ecryptfs_cipher=aes,ecryptfs_key_bytes=16,ecryptfs_passthrough=no,ecryptfs_enable_filename_crypto=${ecryptfs_enable_filename_crypto}
fi;
rm -f tmp.txt
