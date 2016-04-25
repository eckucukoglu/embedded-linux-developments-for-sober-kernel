#ifndef SECURITY_H
#define SECURITY_H

#include "argon2.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum{
    WITHOUT_SHAMIR,
    WITH_SHAMIR
}SHAMIR_TYPE;

#define HASHLEN 32
#define SALTLEN 32
#define SHAMIR_NUMB_OF_SHARE 3
#define SHAMIR_NUMB_OF_THRESHOLD 2

#define DISC_ENC_ECHO_STR "echo "
#define DISC_ENC_PIPE_STR " | "
#define DISC_ENC_ROOT_STR " sudo "
#define DISC_ENC_ADDCRYPT_PATH " ./addCrypt.sh "
#define DISC_ENC_INITCRYPT_PATH " ./initDiskEnc.sh "
#define DISC_ENC_RESETCRYPT_PATH " ./removeCrypt.sh "
#define DISC_ENC_DEV_PATH " /dev/mmcblk0p1 "

int securityGetHashValue(const char *pwd, const unsigned char pwdlen, const void *salt, const unsigned char saltlen, void *hash, const unsigned char hashlen);

int securityCheckPassword(const char *pwd, SHAMIR_TYPE shamirOption);

int securityInitDiscEncryption(const char *pwd, SHAMIR_TYPE shamirOption);

int securityResetDiscEncryption();


#if defined(__cplusplus)
}
#endif

#endif
