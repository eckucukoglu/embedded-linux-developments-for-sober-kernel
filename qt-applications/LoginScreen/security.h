#ifndef SECURITY_H
#define SECURITY_H

#include "argon2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HTTPPerform.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum{
    WITHOUT_SHAMIR,
    WITH_SHAMIR
}SHAMIR_TYPE;

typedef enum
{
    SECURITY_RETURN_OK = 0,
    ERR_SECURITY_RETURN_NOK = 1,
    ERR_SECURITY_SHAMIR_NUMB_OF_THRESHOLD_NOT_REACHED =2,
    ERR_SECURITY_SHAMIR_SERVER_NOT_REACHABLE = 3,
    ERR_SECURITY_DISC_ENC_SALT_FILE_NOT_EXIST =4,
    ERR_SECURITY_DISC_ENC_SALT_LENGTH_ERROR =5,
    ERR_SECURITY_DISC_RESET = 6,
    ERR_HTTP_REQUEST_ERROR = 7
}SECURITY_RETURN_TYPE;

#define HASHLEN 32
#define SALTLEN 32
#define SHAMIR_NUMB_OF_SHARE 3
#define SHAMIR_NUMB_OF_THRESHOLD 2
#define SHAMIR_SERVER_PATH "https://10.155.10.213:"
#define SHAMIR_SERVER_PATH_EXTENSION "001/shamir/" //servers will be in 7001,8001,9001 port

#define DISC_ENC_ECHO_STR "echo "
#define DISC_ENC_PIPE_STR " | "
#define DISC_ENC_ROOT_STR " " // " sudo "
#define DISC_ENC_ADDCRYPT_PATH " /usr/bin/addCrypt.sh "
#define DISC_ENC_INITCRYPT_PATH " /usr/bin/initDiskEnc.sh "
#define DISC_ENC_RESETCRYPT_PATH " /usr/bin/removeCrypt.sh "
#define DISC_ENC_DEV_PATH " /dev/mmcblk0p3 "
//#define DISC_ENC_SALT_FILE_PATH "/etc/crypt/salt"
#define DISC_ENC_SHAMIR_BINARY_PATH "/usr/bin/ssss-combine"


typedef struct  {
    int initialized;
    int shamir;
    int attempt;
    char salt[SALTLEN+1];
}initConf;

extern initConf INITCONFIG;

void securityGetHashValue(const char *pwd, const unsigned char pwdlen, const void *salt, const unsigned char saltlen, void *hash, const unsigned char hashlen);

SECURITY_RETURN_TYPE securityCheckPassword(const char *pwd, SHAMIR_TYPE shamirOption);

SECURITY_RETURN_TYPE securityInitDiscEncryption(const char *pwd, SHAMIR_TYPE shamirOption);

SECURITY_RETURN_TYPE securityResetDiscEncryption();


#if defined(__cplusplus)
}
#endif

#endif
