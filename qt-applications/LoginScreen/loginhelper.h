#ifndef LOGINHELPER_H
#define LOGINHELPER_H
#include <QObject>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dbus/dbus.h>
#include <fstream>
#include <iostream>

#include "security.h"

#define DEBUG_PREFIX "tester: "
#define INIT_MODE_PATH "/etc/crypt/initmode"
#define ATTEMPT_COUNT_PATH "/etc/crypt/attemptcount"
#define INITIALIZED "INITIALIZED"
#define SHAMIR "SHAMIR"
#define ATTEMPT "ATTEMPT"
#define SALT "SALT"
#define INITCONFFILE_PATH "/etc/login.conf"


typedef enum{
    LOGINHELPER_RET_ERROR = -1,
    LOGINHELPER_RET_OK = 0,
}LOGINHELPER_ERR_TYPE;

#define APPMAN_VIEW_DEBUG_PREFIX "L >> "
using namespace std;
class LoginHelper: public QObject
{
    Q_OBJECT
public:
   explicit LoginHelper(QObject *parent = 0);

    Q_INVOKABLE int check_password(QString pwd,bool _isShamir);
    Q_INVOKABLE void query_login(int access_code);
    Q_INVOKABLE void set_attemptCount(int attemptCount);
    Q_INVOKABLE int get_attemptCount();
    Q_INVOKABLE int initDisc(QString password, bool _isShamir);
    Q_INVOKABLE void resetDisc();
    Q_INVOKABLE void set_initMode(int initMode, bool isShamir);
    Q_INVOKABLE int get_initMode();
    Q_INVOKABLE int get_isShamir();
    Q_INVOKABLE void write_initconf(int initialized, int shamir, int attempt);
    Q_INVOKABLE void read_initconf();

private:
    bool isShamir;

};


#endif // LOGINHELPER_H
