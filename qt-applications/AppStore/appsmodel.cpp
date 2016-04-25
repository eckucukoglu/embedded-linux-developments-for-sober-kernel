#include "appsmodel.h"
#define DEBUG_PREFIX "tester: "

AppsModel::AppsModel(QObject *parent) : QObject(parent)
{
    //read list, assign to elementsList
     check_internet = false;
     number_of_installed_applications=0;
     number_of_applications=0;
     error=1;
     current_index= 0;
     page_index=0;
     performer = new HTTPPerform("https://10.155.10.213:443/soberstore");
     query_listapps(); //from device
     listApps(); //from server
     page_count=ceil(double(number_of_applications)/18);
}

int AppsModel::download(int appid)
{
    int _ret=0; //return code
    try {
            _appList = performer->perform(DOWNLOAD,appid);
            if (_appList == NULL)
            {
                cout << "Applist is NULL" << endl;
                error=1;
            }
            else{
                if (performer->getError() == 1){
                    cout << "Error occured during HTTP request :" << performer->getErrorMessage() << endl;
                    error=1;
                }
                else if (_appList->apps != NULL)
                {
                    //TODO: clear this part. you dont have to get app
                    for(int i=0;i< _appList->size; i++) //works for 1 app
                        {
                            cout << "Application "<< to_string(i) << endl;
                            cout << "\t" << "id: " << to_string(_appList->apps[i].id)<<endl;
                            cout << "\t" << "name: " << (_appList->apps[i].name)<<endl;
                            cout << "\t" << "developer name: " << (_appList->apps[i].developerName)<<endl;
                            cout << "\t" << "icon: " << (_appList->apps[i].iconName)<<endl;
                            cout << "\t" << "binary name: "<< (_appList->apps[i].binaryName)<<endl;
                            cout << "\t" << "version: " << (_appList->apps[i].version) << endl;
                        }
                        check_internet=true;
                        if(_appList->apps->isInstalled == 1 && _appList->apps->isDownloaded)
                        {
                            installed_app temp{
                                appid+100,
                                "",
                                ""
                            };
                            INSTALLEDAPPS[number_of_installed_applications] = temp;
                            number_of_installed_applications++;
                            printf("download is successful! - appid: %d\n", appid);
                            error=0;
                        }
                        else
                        {
                            error=1;
                            printf("an error occured while downloading\n");
                        }
                }
                else
                {
                    error=1;
                    cout << "Error occured: " << _appList->apps->errorCode << endl;
                }
            }
        }
        catch(exception &e)
        {
            error=1;
            cout << "Exception!:" << e.what() << endl;
        }
        return _ret;
}

int AppsModel::check_error(){
    return error;
}

void AppsModel::listApps(){
    try {
            _appList = performer->perform(SHOW,0);
            if (_appList == NULL)
            {
                cout << "Applist is NULL" << endl;
            }
            else{
                if (performer->getError() == 1){
                    cout << "Error occured during HTTP request :" << performer->getErrorMessage() << endl;
                    cout << "check 1\n" << endl;
                }
                else if (_appList->apps != NULL)
                {
                    string colours[] = {"#FC0505", "#89F0F0", "#F0E224", "#1AC44D"};
                    number_of_applications = _appList->size;
                    app _list[_appList->size];
                    for(int i=0;i< _appList->size; i++)
                        {
                            cout << "Application "<< to_string(i) << endl;
                            cout << "\t" << "id: " << to_string(_appList->apps[i].id)<<endl;
                            cout << "\t" << "name: " << (_appList->apps[i].name)<<endl;
                            cout << "\t" << "developer name: " << (_appList->apps[i].developerName)<<endl;
                            cout << "\t" << "icon: " << (_appList->apps[i].iconName)<<endl;
                            cout << "\t" << "binaryname: " << (_appList->apps[i].binaryName)<<endl;
                            cout << "\t" << "version:" << (_appList->apps[i].version) << endl;

                            app temp{
                                   _appList->apps[i].id,
                                   _appList->apps[i].name,
                                   _appList->apps[i].developerName,
                                   _appList->apps[i].iconName,
                                   _appList->apps[i].hashValue,
                                   _appList->apps[i].binaryPath,
                                   _appList->apps[i].binaryName,
                                   _appList->apps[i].packagePath,
                                   _appList->apps[i].isDownloaded,
                                   _appList->apps[i].isInstalled,
                                   _appList->apps[i].error,
                                   _appList->apps[i].errorCode,
                                   _appList->apps[i].version,
                                   _appList->apps[i].date,
                                   colours[i%4]
                            };
                            _list[i] = temp;
                        }
                        check_internet=true;
                        set_element_list(_list);
                }
             //   if(appList->apps->isInstalled == 1 && appList->apps->isDownloaded)
             //       cout << "Application installed" << endl;
                else
                    cout << "Error occured: " << _appList->apps->errorCode << endl;
            }
        }
        catch(exception &e)
        {
            cout << "Exception!:" << e.what() << endl;
        }
}



void AppsModel::assert_dbus_method_return (DBusMessage* msg) {
        DBusMessageIter args;
        printf(DEBUG_PREFIX"Coming dbus message...\n");
        printf("\tSender: %s\n", dbus_message_get_sender(msg));
        printf("\tType: %d\n", dbus_message_get_type(msg));
        printf("\tPath: %s\n", dbus_message_get_path(msg));
        printf("\tInterface: %s\n", dbus_message_get_interface(msg));
        printf("\tMember: %s\n", dbus_message_get_member(msg));
        printf("\tDestination: %s\n", dbus_message_get_destination(msg));
        printf("\tSignature: %s\n", dbus_message_get_signature(msg));
        fflush(stdout);

        // TODO: in case of unexpected message type, free objects.

        switch (dbus_message_get_type(msg)) {
        case 0: /* INVALID */
            printf(DEBUG_PREFIX"dbus: invalid message.\n");
            exit(1);
            break;

        case 1: /* METHOD_CALL */
            printf(DEBUG_PREFIX"dbus: method call.\n");
            exit(1);
            break;

        case 2: /* METHOD_RETURN */
            printf(DEBUG_PREFIX"dbus: method return.\n");
            break;

        case 3: /* ERROR */
            printf(DEBUG_PREFIX"dbus: error message.\n");
            char *err_message;
            if (!dbus_message_iter_init(msg, &args)) {
                fprintf(stderr, DEBUG_PREFIX"dbus: message has no arguments.\n");
            } else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) {
                fprintf(stderr, DEBUG_PREFIX"dbus: argument is not string.\n");
            } else
                dbus_message_iter_get_basic(&args, &err_message);

            printf(DEBUG_PREFIX"dbus: %s.\n", err_message);
            exit(1);
            break;

        case 4: /* SIGNAL */
            printf(DEBUG_PREFIX"dbus: signal.\n");
            exit(1);
            break;

        default:
            printf(DEBUG_PREFIX"dbus: unknown message.\n");
            exit(1);
            break;
        }
    }



/**
 * For checking if app already installed
 */
void AppsModel::query_listapps() {
     DBusMessage* msg;
     DBusMessageIter args, arrayIter, structIter;
     DBusConnection* conn;
     DBusError err;
     DBusPendingCall* pending;
     int ret;
     int i;

     // initialiset the errors
     dbus_error_init(&err);

     // connect to the system bus and check for errors
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
     ret = dbus_bus_request_name(conn, "appman.method.appstoreview",
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
                                        "listapps"); // method name

     if (NULL == msg) {
         fprintf(stderr, DEBUG_PREFIX"dbus: out of memory.\n");
         exit(1);
     }

     // send message and get a handle for a reply
     if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) {
         fprintf(stderr, DEBUG_PREFIX"dbus: out of memory.\n");
         exit(1);
     }

     if (!pending) {
         fprintf(stderr, DEBUG_PREFIX"dbus: null pending call.\n");
         exit(1);
     }

     dbus_connection_flush(conn);

     // free message
     dbus_message_unref(msg);

     // block until we recieve a reply
     dbus_pending_call_block(pending);

     // get the reply message
     msg = dbus_pending_call_steal_reply(pending);

     if (NULL == msg) {
         fprintf(stderr, DEBUG_PREFIX"dbus: null reply.\n");
         exit(1);
     }

     // free the pending message handle
     dbus_pending_call_unref(pending);

     /* Be sure that dbus message is method return. */
     assert_dbus_method_return(msg);

     // read the parameters
     if (!dbus_message_iter_init(msg, &args)) {
         fprintf(stderr, DEBUG_PREFIX"dbus: message has no arguments.\n");
     } else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) {
         fprintf(stderr, DEBUG_PREFIX"dbus: argument is not integer.\n");
     } else
         dbus_message_iter_get_basic(&args, &number_of_installed_applications);

     printf(DEBUG_PREFIX"#installed applications: %d.\n", number_of_installed_applications);

     if (!dbus_message_iter_next(&args)) {
         fprintf(stderr, DEBUG_PREFIX"dbus: message has too few arguments.\n");
     } else if (DBUS_TYPE_ARRAY != dbus_message_iter_get_arg_type(&args)) {
         fprintf(stderr, DEBUG_PREFIX"dbus: argument is not array.\n");
     } else {
         dbus_message_iter_recurse(&args, &arrayIter);

         for (i = 0; i < number_of_installed_applications; ++i) {
             if (DBUS_TYPE_STRUCT == dbus_message_iter_get_arg_type(&arrayIter)) {
                 dbus_message_iter_recurse(&arrayIter, &structIter);
                 if (DBUS_TYPE_UINT32 == dbus_message_iter_get_arg_type(&structIter))
                     dbus_message_iter_get_basic(&structIter, &(INSTALLEDAPPS[i].id));

                 dbus_message_iter_next(&structIter);

                 if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&structIter))
                     dbus_message_iter_get_basic(&structIter, &(INSTALLEDAPPS[i].prettyname));

                 dbus_message_iter_next(&structIter);

                 if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&structIter))
                     dbus_message_iter_get_basic(&structIter, &(INSTALLEDAPPS[i].icon));
             }

             dbus_message_iter_next(&arrayIter);
         }
     }
     // unref message and connection
     dbus_message_unref(msg);
     dbus_connection_unref(conn);
}

int AppsModel::check_if_installed(int app_id)
{
     int ret=0;
     for(int i=0;i<number_of_installed_applications;i++)
     {
         if(INSTALLEDAPPS[i].id == (app_id+100))
         {
             ret=1;
             break;
         }
     }
     return ret;
}

string format_version(double version){
    std::ostringstream versionDouble;
    versionDouble << version;
    std::string versionStr = versionDouble.str();
    return versionStr;
}

string format_date(int date){
    struct tm * timeinfo;

    /* Conversion to time_t as localtime() expects a time_t* */
    time_t epoch_time_as_time_t = date;

    /* Call to localtime() now operates on time_t */
    timeinfo = localtime(&epoch_time_as_time_t);

    char buffer[32];
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buffer, 32, "%d.%m.%Y", timeinfo);
    std::string dateStr(buffer);
    return dateStr;
}

void AppsModel::set_element_list(app _list[]){
    QVariantList _list1;
    for(int i=0; i<number_of_applications;i++)
    {
        QVariant _data;
        QVariantMap _map;
        int _isInstalled=check_if_installed(_list[i].id);
        printf("Desktopscreen: app: %d is installed :%d\n",_list[i].id, _isInstalled);
        _map["id"] = QVariant(_list[i].id);
        _map["name"] = QVariant(_list[i].name.c_str());
        _map["developerName"] = QVariant(_list[i].developerName.c_str());
        _map["iconName"] = QVariant(_list[i].iconName.c_str());
        _map["hashValue"] = QVariant(_list[i].hashValue.c_str());
        _map["binaryPath"] = QVariant(_list[i].binaryPath.c_str());
        _map["binaryName"] = QVariant(_list[i].binaryName.c_str());
        _map["packagePath"] = QVariant(_list[i].packagePath.c_str());
        _map["isDownloaded"] = QVariant(_list[i].isDownloaded);
        _map["isInstalled"] = QVariant(_list[i].isInstalled);
        _map["error"] = QVariant(_list[i].error);
        _map["errorCode"] = QVariant(_list[i].errorCode.c_str());
        _map["borderColor"] = QVariant(_list[i].borderColor.c_str());
        _map["alreadyInstalled"] =QVariant(_isInstalled);
        _map["version"] = QVariant(format_version(_list[i].version).c_str());
        _map["date"] = QVariant(format_date(_list[i].date).c_str());

        _data = QVariant(_map);
        _list1.append(_data);
    }
    appList = QVariant(_list1);
    set_page_count();
}



QVariant AppsModel::get_element_list()
{
   return appList;
}

int AppsModel::get_current_index()
{
   return current_index;
}
void AppsModel::set_current_index(int index)
{
   current_index = index;
}
int AppsModel::get_page_index()
{
    return page_index;
}
void AppsModel::set_page_index(int index)
{
    page_index= index;
}
bool AppsModel::check_connection(){
    return check_internet;
}

int AppsModel::get_number_of_applications()
{
    return number_of_applications;
}
int AppsModel::get_applist()
{
    return APPLIST[1].id;
}

void AppsModel::set_page_count()
{
    page_count = ceil(double(number_of_applications)/18);
}
int AppsModel::get_page_count()
{
    return page_count;
}
