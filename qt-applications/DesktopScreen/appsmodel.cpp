#include "appsmodel.h"
#define DBUS_API_SUBJECT_TO_CHANGE
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <QTimer>

AppsModel::AppsModel(QObject *parent) : QObject(parent)
{
    //read list, assign to elementsList
    current_index = 0;
    page_count = 0;
    page_index = 0;
    _isActive = true;
    query_listapps();
}



/*
AppsModel::~AppsModel()
{

}
*/
void AppsModel::set_element_list(application_desktop _list[]){
    QVariantList _list1;
    string colours[] = {"#0e95c5", "#e89e0c", "#88aedb", "#fd9f1b", "#1abc9c", "#f37021", "#2aaf5c", "#e74c3c", "#b80505"};
    for(int i=0; i<number_of_applications;i++)
    {
        QVariant _data;
        QVariantMap _map;
        _map["prettyname"] = QVariant(_list[i].prettyname);
        _map["icon"] = QVariant(_list[i].icon);
        _map["id"] = QVariant(_list[i].id);
        _map["color"] = QVariant(colours[i%9].c_str());
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
bool AppsModel::is_active()
{
   return _isActive;
}
void AppsModel::set_is_active(bool _value)
{

   _isActive = _value;
}
int AppsModel::get_page_index()
{
    return page_index;
}
void AppsModel::set_page_index(int index)
{
    page_index= index;
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
void AppsModel::shutdown(){
    system("poweroff") ;
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
 * Call a listapps method on a remote object.
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
    ret = dbus_bus_request_name(conn, "appman.method.view",
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
        dbus_message_iter_get_basic(&args, &number_of_applications);

    printf(DEBUG_PREFIX"#applications: %d.\n", number_of_applications);

    if (!dbus_message_iter_next(&args)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: message has too few arguments.\n");
    } else if (DBUS_TYPE_ARRAY != dbus_message_iter_get_arg_type(&args)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: argument is not array.\n");
    } else {
        dbus_message_iter_recurse(&args, &arrayIter);

        for (i = 0; i < number_of_applications; ++i) {
            if (DBUS_TYPE_STRUCT == dbus_message_iter_get_arg_type(&arrayIter)) {
                dbus_message_iter_recurse(&arrayIter, &structIter);
                if (DBUS_TYPE_UINT32 == dbus_message_iter_get_arg_type(&structIter))
                    dbus_message_iter_get_basic(&structIter, &(APPLIST[i].id));

                dbus_message_iter_next(&structIter);

                if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&structIter))
                    dbus_message_iter_get_basic(&structIter, &(APPLIST[i].prettyname));

                dbus_message_iter_next(&structIter);

                if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&structIter))
                    dbus_message_iter_get_basic(&structIter, &(APPLIST[i].icon));

                printf(DEBUG_PREFIX"%s(%d) received.\n", APPLIST[i].prettyname, APPLIST[i].id);
            }

            dbus_message_iter_next(&arrayIter);
        }
    }

    // unref message and connection
    dbus_message_unref(msg);
    dbus_connection_unref(conn);
    set_element_list(APPLIST);
}


/**
 * Call a runapp method on a remote object.
 */
int AppsModel::query_startapp(int app_id) {
    DBusMessage* msg;
    DBusMessageIter args;
    DBusConnection* conn;
    DBusError err;
    DBusPendingCall* pending;
    int ret;
    dbus_uint32_t run_ret;

    // initialiset the errors
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
    ret = dbus_bus_request_name(conn, "appman.method.view",
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
                  "startapp"); // method name

    if (!msg) {
        fprintf(stderr, DEBUG_PREFIX"dbus: null message.\n");
        exit(1);
    }

    // append arguments
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &app_id)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: out of memory.\n");
        exit(1);
    }

    // send message and get a handle for a reply
    if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) { // -1 is default timeout
        fprintf(stderr, DEBUG_PREFIX"dbus: out of memory.\n");
        exit(1);
    }

    if (NULL == pending) {
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

    assert_dbus_method_return(msg);

    // read the parameters
    if (!dbus_message_iter_init(msg, &args)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: message has no arguments.\n");
    } else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: argument is not integer.\n");
    } else
        dbus_message_iter_get_basic(&args, &run_ret);

    printf(DEBUG_PREFIX"dbus: reply message %d.\n", run_ret);

    dbus_message_unref(msg);
    dbus_connection_unref(conn);
    return run_ret;
}
/**
 * Call a access method on a remote object.
 */
void query_access(int access_code) {
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
                                    "access"); // method name

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

/**
 * Call updateapps method on a remote object.
 */
/*
void query_updateapps() {
    DBusMessage* msg;
    DBusConnection* conn;
    DBusError err;
    int ret;

    // initialize errors
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
    ret = dbus_bus_request_name(conn, "appman.method.appstore",
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
                                    "updateapps"); // method name

    if (!msg) {
        fprintf(stderr, DEBUG_PREFIX"dbus: null message.\n");
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
*/
void AppsModel::_securityResetDiscEncryption(){
    securityResetDiscEncryption();
}

void AppsModel::query_lockscreen() {
    DBusMessage* msg;
    DBusConnection* conn;
    DBusError err;
    int ret;

    // initialize errors
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
    ret = dbus_bus_request_name(conn, "appman.method.view",
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
                                    "lockscreen"); // method name

    if (!msg) {
        fprintf(stderr, DEBUG_PREFIX"dbus: null message.\n");
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


int AppsModel::query_uninstallapps(unsigned int* app_ids, int size) {
    DBusMessage* msg;
    DBusMessageIter args, arrayIter;
    DBusConnection* conn;
    DBusError err;
    DBusPendingCall* pending;
    dbus_uint32_t *v_ARRAY;
    int ret, i;
    unsigned int removed_app_ids[MAX_NUMBER_APPLICATIONS];
    int number_of_removed_apps = 0;

    // initialiset the errors
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
    ret = dbus_bus_request_name(conn, "appman.method.view",
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
                                      "uninstallapps"); // method name

    if (!msg) {
        fprintf(stderr, DEBUG_PREFIX"dbus: null message.\n");
        exit(1);
    }

    // append arguments
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &size)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: out of memory.\n");
        exit(1);
    }

    dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "u", &arrayIter);

    v_ARRAY = app_ids;
    if (!dbus_message_iter_append_fixed_array (&arrayIter, DBUS_TYPE_UINT32, &v_ARRAY, size)) {
        fprintf (stderr, "No memory!\n");
    }

    dbus_message_iter_close_container(&args, &arrayIter);

    // send message and get a handle for a reply
    if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) { // -1 is default timeout
        fprintf(stderr, DEBUG_PREFIX"dbus: out of memory.\n");
        exit(1);
    }

    if (NULL == pending) {
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

    assert_dbus_method_return(msg);

    // read the parameters
    if (!dbus_message_iter_init(msg, &args)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: message has no arguments.\n");
    } else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: argument is not integer.\n");
    } else
        dbus_message_iter_get_basic(&args, &number_of_removed_apps);

    if (!dbus_message_iter_next(&args)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: message has too few arguments.\n");
    } else if (DBUS_TYPE_ARRAY != dbus_message_iter_get_arg_type(&args)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: argument is not array.\n");
    } else {
        dbus_message_iter_recurse(&args, &arrayIter);
        for (i = 0; i < number_of_removed_apps; ++i) {
            if (DBUS_TYPE_UINT32 == dbus_message_iter_get_arg_type(&arrayIter)) {
                dbus_message_iter_get_basic(&arrayIter, removed_app_ids+i);
            }

            dbus_message_iter_next(&arrayIter);
        }
    }

    printf(DEBUG_PREFIX"Removed %d applications: ", number_of_removed_apps);
    for (i = 0; i < number_of_removed_apps; i++) {
        printf("%d ", removed_app_ids[i]);
    }
    printf("\n");

    dbus_message_unref(msg);
    dbus_connection_unref(conn);
    return removed_app_ids[0];
}



bool AppsModel::deleteApp(int app_id)
{
     printf("Desktopscreen: deleting app with ID: %d\n", app_id);
     unsigned int app[1];
     app[0] = (unsigned int)app_id;
     int appsize = 1;
     int deletedapp = query_uninstallapps(app, appsize);
     printf("Desktopscreen: deleted app with ID: %d\n", deletedapp);
     if(deletedapp == app_id)
         return true;
     return false;
}

