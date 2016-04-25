#!/bin/sh

passwordFileNotFound(){
	echo "Password file is NOT found!";
}

if [ -z ${1} ] || [ -z ${2} ] || [ -z ${3} ]
then 
	echo "Usage of this script is: ${0} /path/to/file/to/encrypt /path/to/encrypted/file [PASSWORD_FILE]"
else
	toBeEncryptedFilePath=${1}
	encryptedFile=${2}
	passwordFile=${3}
		
	if [ -f $passwordFile ]
	then
		while read line
		do
		    name=$line
		    password=$line
		    break
		done < $passwordFile

		if [ -z $password ];
		then 
			# Password file exist, but is emtpy!			
			echo "Password is not set, wrong File!";
		else
			openssl enc -aes-128-cbc -nosalt -pass pass:$password -in $toBeEncryptedFilePath -out $encryptedFile
		fi
	else
		# Password file is NOT exist
		echo "Password file is NOT found!"
	fi
fi
