#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "security.h"
#include "HTTPPerform.h"

unsigned int t_cost = (1<<3);   // 3-pass computation
unsigned int m_cost = (1<<12);  // 2^12 4 mebibytes memory usage
unsigned int parallelism = 1;   // number of threads and lanes


typedef enum
{
    DECRYPTION = 0,
    ENCRYPTION = 1,
}DISCOPERATION_TYPE;

void securityGetCommandLineOutput(const char* cmd,char* salt)
{
    //printf("Command to Execute:[%s] \n", cmd);
    FILE *pp = popen(cmd, "r");
    fgets(salt, SALTLEN + 1, pp);
    pclose(pp);
    return;
}

int securityDiscOperation(const unsigned char *hash, const char* discPath,DISCOPERATION_TYPE discOp)
{
    char cmd[100];
    strcpy(cmd,DISC_ENC_ECHO_STR);
    strcat(cmd,"\'\'\'"); strcat(cmd,(char*)hash); strcat(cmd,"\'\'\'");
    strcat(cmd,DISC_ENC_PIPE_STR);
    strcat(cmd,DISC_ENC_ROOT_STR);
    if (discOp == DECRYPTION) strcat(cmd,DISC_ENC_ADDCRYPT_PATH);
    else strcat(cmd,DISC_ENC_INITCRYPT_PATH);
    strcat(cmd,DISC_ENC_DEV_PATH);

    //printf("Command that will be executed : %s \n",cmd);

    //int i, discRetVal = system(cmd);
    //i=WEXITSTATUS(discRetVal);

    int i = system(cmd);

    //printf("Disk Operation Status = %s\n",i==0?"OK":"NOK");

    return i;
}

int securityGetHashValue(const char *pwd, const unsigned char pwdlen, const void *salt, const unsigned char saltlen, void *hash, const unsigned char hashlen)
{
    argon2i_hash_raw(t_cost, m_cost, parallelism, pwd, pwdlen, salt, saltlen, hash, hashlen);
    //for( int i=0; i<hashlen; ++i ) printf( "%02x", hash[i] ); printf( "\n" );
    return 0;
}

int securityGetSaltValue(char *salt, SHAMIR_TYPE shamirOption)
{
    //get salt value from shamir or from local disk
    memset( salt, 0x00, SALTLEN +1 );


    if (shamirOption == WITH_SHAMIR)
    {
        //get salt from servers

        HTTPPerform *performer;
        string share="";
        string url = "";
        string command = "";
        int shareCount = 0;

        for ( int i = 0; i < SHAMIR_NUMB_OF_SHARE; i++)
        {
            share="";
            try {
                url = "http://10.155.10.213:808" + to_string(i) + "/server/";
                performer  = new HTTPPerform(url);
                performer->perform(GETSHARE,share);

                if (performer->getError() == 1)
                {
                    cout << "Error occured during HTTP request :" << performer->getErrorMessage() << endl;
                    performer->~HTTPPerform();
                }
                else
                {
                    //cout << "HTTP request OK share :" << share << endl;
                    shareCount++;
                    command += (share + "\n");
                    performer->~HTTPPerform();

                    if (shareCount >= SHAMIR_NUMB_OF_THRESHOLD)		break;
                }
            }

            catch(exception &e)
            {
                cout << "Exception!:" << e.what() << endl;
            }

        }

        if (shareCount < SHAMIR_NUMB_OF_THRESHOLD)
        {
            cout << "Server Connection Error!!! Share threshold value could not be reached !!!" << endl;
            return -1;
        }

        command = "printf \"" + command + "\" | ./ssss-combine -t " + to_string(SHAMIR_NUMB_OF_THRESHOLD);
        securityGetCommandLineOutput(command.c_str(),salt);

    }
    else
    {
        //get salt from local storage
        FILE *fp;

        fp = fopen("./salt.txt", "rt");
        if (!fp)
            printf("Salt Error!!! Salt file does not exist!!! Default Salt will be used...\n");
        else
        {
            //printf(	"localSalt lenght:%d sizeof :%d \n",strlen(localSalt), sizeof(localSalt));
            if (fgets(salt, SALTLEN + 1 , fp) == NULL)
                printf("Salt Error!!! Salt value error!!! Salt Lenght:%d \n",(int)strlen(salt));
            salt[SALTLEN] = '\0';

        }

    }
    printf("\nSalt that will be used : %s \n",salt );
    return 0;
}

int securityCheckPassword(const char *pwd, SHAMIR_TYPE shamirOption)
{
    unsigned char hash[HASHLEN + 1];
    char salt[SALTLEN+1];

    securityGetSaltValue(salt, shamirOption);
    securityGetHashValue(pwd, strlen(pwd), (unsigned char*)salt, SALTLEN, hash, HASHLEN );
    hash[HASHLEN] = '\0';

    //char hashHex[HASHLEN*2];
    //for( int i=0; i<HASHLEN; ++i ) sprintf(&hashHex[i*2], "%02x", hash[i]); printf("Hash  Hex: %s\n",hashHex);

    return securityDiscOperation(hash,DISC_ENC_DEV_PATH,DECRYPTION);
}

int securityInitDiscEncryption(const char *pwd, SHAMIR_TYPE shamirOption)
{

    unsigned char hash[HASHLEN + 1];
    char salt[SALTLEN+1];

    securityGetSaltValue(salt, shamirOption);
    securityGetHashValue(pwd, strlen(pwd), (unsigned char*)salt, SALTLEN, hash, HASHLEN );
    hash[HASHLEN] = '\0';

    //char hashHex[HASHLEN*2];
    //for( int i=0; i<HASHLEN; ++i ) sprintf(&hashHex[i*2], "%02x", hash[i]); printf("Hash  Hex: %s\n",hashHex);

    return securityDiscOperation(hash,DISC_ENC_DEV_PATH,ENCRYPTION);

}

int securityResetDiscEncryption()
{
    char cmd[50];

    strcpy(cmd,DISC_ENC_ROOT_STR);
    strcat(cmd,DISC_ENC_RESETCRYPT_PATH);

    //printf("Command that will be executed : %s \n",cmd);
    int i = system(cmd);
    //printf("Disk Operation Status = %s\n",i==0?"OK":"NOK");

    return i;
}

/*
int checkArgs( int argc, char *argv[] )
{
   if( argc == 4 ) {
    int iteration = atoi(argv[1]);
    t_cost = (1 << iteration);

    int memory = atoi(argv[2]);
    m_cost = (1 << memory);

    parallelism = atoi(argv[3]);

    printf("Iteration:\t%d \t t_cost: %d \n", iteration, t_cost);
    printf("Memory:\t\t%d \t m_cost: %d \n", memory, m_cost);
    printf("parallelism:\t%d\n", parallelism);
    return 0;
   }
   else if( argc > 2 ) {
    printf("Too many arguments supplied.\n");
    return -1;
   }
   return 0;
}
*/
/*
int main( int argc, char *argv[] )
{
    int result;
    //SHAMIR_TYPE shamirOption;

    if (checkArgs(argc, argv) < 0)
        return -1;

    //char *pwd = (char *)strdup(PWD);
    char pwd[] = "merhaba";

    //shamirOption = withShamir; // set shamirOption according to user preferences

    result = securityCheckPassword(pwd, WITHOUT_SHAMIR);
    //result = securityInitDiscEncryption(pwd, WITHOUT_SHAMIR);

    //result = securityResetDiscEncryption();

    if (result == 0) printf("=> RESULT : Password is OK...\n");
    else printf("=> RESULT : Password is NOT OK...\n");

    securityGetCommandLineOutput();

    return 0;
}
*/
