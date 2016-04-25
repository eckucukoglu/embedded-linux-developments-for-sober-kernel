#include "tester.h"

void assert_dbus_method_return (DBusMessage* msg) {
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

void query_startapp(int app_id) {
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
}

void query_listapps() {
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
}

void query_uninstallapps(unsigned int* app_ids, int size) {
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
}

void query_login(int access_code) {
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

void query_lockscreen() {
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

int main(int argc, char** argv) {

    if (argv[1] != NULL) {
        if (strcmp("startapp", argv[1]) == 0 && argv[2])
            query_startapp(atoi(argv[2]));
        else if (strcmp(argv[1], "listapps") == 0)
            query_listapps();
        else if (strcmp(argv[1], "uninstallapps") == 0) {
            int i, size = 4;
            unsigned int* apps_to_remove = malloc(sizeof(unsigned int) * size);
            for (i = 0; i < size; i++) {
                apps_to_remove[i] = 100 + (i * size);
            }        
        
            query_uninstallapps(apps_to_remove, size);
            free(apps_to_remove);
        }
        else if (strcmp(argv[1], "login") == 0)
            query_login(0);
        else if (strcmp(argv[1], "updateapps") == 0)
            query_updateapps();
        else if (strcmp(argv[1], "lockscreen") == 0)
            query_lockscreen();
        else {
            goto USAGE;
        }    
    
    } else {
USAGE:  printf ("Usage: %s [startapp|listapps|uninstallapps|"
                "login|updateapps|lockscreen] [<param>]\n", argv[0]);
        return 1;
    }

    return 0;
}
