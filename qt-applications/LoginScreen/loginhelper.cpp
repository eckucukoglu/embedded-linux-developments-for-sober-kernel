#include "loginhelper.h"

LoginHelper::LoginHelper(QObject *parent) : QObject(parent)
{
       read_initconf();
}

int LoginHelper::check_password(QString password, bool _isShamir){
    char* pwd = new char[password.length() + 1];
    strcpy(pwd, password.toStdString().c_str());
    isShamir = _isShamir;
    int result = 1;
    if(isShamir)
    {
        result = securityCheckPassword(pwd, WITH_SHAMIR);
        printf("=>WITH SHAMIR...\n");

    }
    else
    {
        result = securityCheckPassword(pwd, WITHOUT_SHAMIR);
        printf("=>WITHOUT SHAMIR...\n");
    }

    if (result == SECURITY_RETURN_OK){
        printf("=> RESULT : Password is OK...\n");
    }
    else
    {
        printf("=> RESULT : Password is NOT OK...\n");
    }

    return result;
}


void LoginHelper::query_login(int access_code) {
    DBusMessage* msg;
    DBusMessageIter args;
    DBusConnection* conn;
    DBusError err;
    int ret;

    // initialize the errors
    dbus_error_init(&err);

    // connect to the session bus and check for errors
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: connection error: %s.\n", err.message);
        dbus_error_free(&err);
    }

    if (!conn) {
        printf(DEBUG_PREFIX"dbus: null connection.\n");
        exit(1);
    }

    // request our name on the bus
    ret = dbus_bus_request_name(conn, "appman.method.login",
                                DBUS_NAME_FLAG_REPLACE_EXISTING , &err);

    if (dbus_error_is_set(&err)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: name error: %s.\n", err.message);
        dbus_error_free(&err);
    }

    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret &&
        DBUS_REQUEST_NAME_REPLY_ALREADY_OWNER != ret) {
        printf(DEBUG_PREFIX"dbus: name owner error.\n");
        exit(1);
    }

    // create a new method call and check for errors
    msg = dbus_message_new_method_call("appman.method.server", // target for the method call
                                    "/appman/method/Object", // object to call on
                                    "appman.method.Type", // interface to call on
                                    "login"); // method name

    if (!msg) {
        fprintf(stderr, DEBUG_PREFIX"dbus: null message.\n");
        exit(1);
    }

    // append arguments
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &access_code)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: out of memory.\n");
        exit(1);
    }

    // send message
    if (!dbus_connection_send(conn, msg, NULL)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: out of memory.\n");
        exit(1);
    }

    dbus_connection_flush(conn);

    dbus_message_unref(msg);
    dbus_connection_unref(conn);
}

void LoginHelper::resetDisc()
{
       securityResetDiscEncryption();
}

int LoginHelper::initDisc(QString password, bool _isShamir){
    char* pwd = new char[password.length() + 1];
    strcpy(pwd, password.toStdString().c_str());
    isShamir = _isShamir;
    int result = 1;
    if(isShamir)
    {
        result = securityInitDiscEncryption(pwd, WITH_SHAMIR);
        printf("=>WITH SHAMIR...\n");

    }
    else
    {
        result = securityInitDiscEncryption(pwd, WITHOUT_SHAMIR);
        printf("=>WITHOUT SHAMIR...\n");
    }

    if (result == SECURITY_RETURN_OK){
        printf("=> RESULT : Initialize is OK...\n");
    }
    else{
        printf("=> RESULT : Initialize is NOT OK...\n");
    }
    return (int)result;
}

void LoginHelper::set_attemptCount(int attemptCount)
{
    INITCONFIG.attempt = attemptCount;
    write_initconf(INITCONFIG.initialized, INITCONFIG.shamir, attemptCount);
}


void LoginHelper::set_initMode(int initMode, bool isShamir)
{
    INITCONFIG.initialized = initMode;
    INITCONFIG.shamir = isShamir;
    write_initconf(initMode, isShamir, INITCONFIG.attempt);
}

int LoginHelper::get_initMode(){
    return INITCONFIG.initialized;
}

int LoginHelper::get_isShamir(){
    return INITCONFIG.shamir;
}

int LoginHelper::get_attemptCount()
{
    return INITCONFIG.attempt;
}

void LoginHelper::read_initconf(){
    std::ifstream file(INITCONFFILE_PATH);
    std::string str;
    std::string delimiter = "=";
    std::string tag;
    std::string token;
    while (std::getline(file, str))
    {
       tag = str.substr(0, str.find(delimiter));
       token = str.substr(str.find(delimiter)+1, str.length());
       if(tag == INITIALIZED)
       {
            INITCONFIG.initialized = atoi(token.c_str());
       }
       else if(tag == SHAMIR)
       {
            INITCONFIG.shamir = atoi(token.c_str());
       }
       else if(tag == ATTEMPT)
       {
            INITCONFIG.attempt = atoi(token.c_str());
       }
       else if(tag == SALT)
       {
            strcpy(INITCONFIG.salt ,token.c_str());
       }
    }

    cout << INITIALIZED << ":" << INITCONFIG.initialized << endl;
    cout << SHAMIR << ":" << INITCONFIG.shamir << endl;
    cout << ATTEMPT << ":" << INITCONFIG.attempt << endl;
    cout << SALT << ":" << INITCONFIG.salt << endl;
}

void LoginHelper::write_initconf(int initialized, int shamir, int attempt)
{
   std::ofstream output;
   output.open (INITCONFFILE_PATH, std::ofstream::out);
   output << INITIALIZED << "=" << initialized << endl;
   output << SHAMIR << "=" << shamir << endl;
   output << ATTEMPT << "=" << attempt << endl;
   output << SALT << "=" << INITCONFIG.salt << endl;
   output.close();
}
