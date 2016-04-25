#include "appmand.h"

/* Thread that grabs mutex, must be the same thread that release mutex. */
pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
/* Means there exists children to wait. */
pthread_cond_t child_exists = PTHREAD_COND_INITIALIZER;

char* const system_apps[][3] = {
    /* Path, name, hash. */
    { "/usr/bin/LoginScreen", "LoginScreen", "" },
    { "/usr/bin/DesktopScreen", "DesktopScreen", "" },
    { "/usr/bin/Soberstore", "Soberstore", "" }
};

const char *reasonstr(int signal, int code) {
    if (signal == SIGCHLD) {
        switch(code) {
        case CLD_EXITED:
            return("CLD_EXITED");
        case CLD_KILLED:
            return("CLD_KILLED");
        case CLD_DUMPED:
            return("CLD_DUMPED");
        case CLD_TRAPPED:
            return("CLD_TRAPPED");
        case CLD_STOPPED:
            return("CLD_STOPPED");
        case CLD_CONTINUED:
            return("CLD_CONTINUED");
        }
    }

    return "unknown";
}

bool application_integrity_check(int appindex) {
    FILE* file;
    int i, size;
    unsigned char* buffer;
    unsigned char digest[SHA256_DIGEST_LENGTH];
    char hash[65];
    
    file = fopen(APPLIST[appindex].path, "r");
    
    if (!file)
        return false;
    
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    buffer = malloc(size);
    
    if (!buffer)
        return false;
        
    if (size != fread(buffer, 1, size, file))
        return false;

    if (!SHA256(buffer, size, digest))
        return false;
    
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(hash+(i*2), "%02x", digest[i]);
    hash[i*2] = '\0';
    
    free(buffer);
    fclose(file);
    
    return strcmp(hash, APPLIST[appindex].hash) == 0;
}

pid_t run(const char *path, const char *name) {
    int rc;
    pid_t pid = fork();

    if (pid == 0) {
        rc = execl(path, name, (char*)NULL);
        
        if (rc == -1) {
            sleep(1);
            handle_error("Could not execl");
        }
    } else if (pid < 0) {
        handle_error("fork");
    }
    
#ifdef DEBUG
    printf(DEBUG_PREFIX"%s(%d) launched.\n", name, pid);
    fflush(stdout);
#endif
    
    return pid;
}

int assign_control_group(pid_t pid, const char *group) {
    if (strcmp(group, "none") == 0)
        return 0;
    
    FILE *file;
    char *group_tasks_path;
    int pathname_length;
    char pid_str[5];
    
    sprintf(pid_str, "%d", pid);

    pathname_length = strlen(CGROUPS_VFS) + 
                      strlen(group) + 
                      strlen("/tasks") + 1;
    group_tasks_path = malloc(pathname_length);
    if (!group_tasks_path)
        handle_error("malloc for group_tasks_path");
    
    strcpy(group_tasks_path, CGROUPS_VFS);
    strcat(group_tasks_path, group);
    strcat(group_tasks_path, "/tasks");
    
    file = fopen(group_tasks_path, "r+");
#ifdef DEBUG
    if (file) {
        printf(DEBUG_PREFIX"%s valid path.\n", group_tasks_path);
        fflush(stdout);
    }
#endif
    if (!file)
        return -1;
    
    if(fputs(pid_str, file) < 0)
        return -1;
    
    if (group_tasks_path) free(group_tasks_path);
    fclose(file);
    return 0;
}


int json_to_application (char *text, int index) {
    cJSON *root, *child;
    char* fields[7];
    int i;
    unsigned int id;
    
    root = cJSON_Parse(text);
    
    if (!root) {
        printf(DEBUG_PREFIX"cJSON error: %s.\n", cJSON_GetErrorPtr());
        return -1;
    } else {
        char field_names[7][50] = {
            "id",
            "path",
            "name",
            "group",
            "prettyname",
            "icon",
            "hash"
        };
        
        i = 0;
        while (i < 7) {
            child = cJSON_GetObjectItem(root, field_names[i]);
            if (child != NULL) {
                if (i == 0)
                    id = child->valueint;
                else {
                    fields[i] = malloc(256);                
                    strcpy(fields[i], child->valuestring);
                }
            } else {
                printf(DEBUG_PREFIX"Can not find json object %s.\n", field_names[i]);
            }
            
            i++;
        }
        
        for (i = 0; i < index; i++) {
            if (APPLIST[i].id == id) {
                cJSON_Delete(root);
                return -1;
            }
        }
        
        // TODO: Check exceptions, assign default.
        APPLIST[index].id = id;
        APPLIST[index].path = fields[1];
        APPLIST[index].name = fields[2];
        APPLIST[index].group = fields[3];
        APPLIST[index].prettyname = fields[4];
        APPLIST[index].icon = fields[5];
        APPLIST[index].hash = fields[6];
        APPLIST[index].pid = -1;
        
#ifdef DEBUG
        printf(DEBUG_PREFIX"%s(%d) found.\n", 
               APPLIST[index].prettyname, APPLIST[index].id);
        fflush(stdout);
#endif
        cJSON_Delete(root);
    }
    
    return 0;
}

int add_system_apps (int index) {
    int i;
    
    // check existence
    for (i = 0; i < index; i++) {
        if (APPLIST[i].id == SOBERSTORE_APP_ID)
            return 0;
    }
    
    /* Soberstore. */
    APPLIST[index].id = SOBERSTORE_APP_ID;
    APPLIST[index].path = system_apps[SOBERSTORE][0];
    APPLIST[index].name = system_apps[SOBERSTORE][1];
    APPLIST[index].group = "none";
    APPLIST[index].prettyname = system_apps[SOBERSTORE][1];
    APPLIST[index].icon = "";
    APPLIST[index].hash = system_apps[SOBERSTORE][2];
    APPLIST[index].pid = -1;
    
    return 1;
}
        
int removefile (char* filepath) {
    int ret = remove(filepath);
    
    if (ret == 0) {
#ifdef DEBUG
        printf(DEBUG_PREFIX"%s deleted successfully.\n", filepath);
        fflush(stdout);
#endif    
    } else {
        fprintf(stderr, DEBUG_PREFIX"Error deleting the file %s.\n", filepath);
        fflush(stderr);
    }
    
    return ret;
}

int removeapp(int app_id) {
    int i;
    
    // Do not remove Soberstore application.
    if (app_id == SOBERSTORE_APP_ID)
        return -1;
    
    for (i = 0; i < number_of_applications; i++) {
        if (APPLIST[i].id == app_id) {
            int retval;
            char *manifest_filepath;
            char app_id_str[4];
            int pathname_length;
            
            sprintf(app_id_str, "%d", app_id);
            pathname_length = strlen(MANIFEST_DIR) + 
                              strlen(app_id_str) + 
                              strlen(".mf") + 1;
            manifest_filepath = malloc(pathname_length);
            if (!manifest_filepath)
                handle_error("malloc for manifest_filepath");
                
            strcpy(manifest_filepath, MANIFEST_DIR);
            strcat(manifest_filepath, app_id_str);
            strcat(manifest_filepath, ".mf");
            
            if (removefile(manifest_filepath) == 0 && 
                removefile(APPLIST[i].path) == 0) {
#ifdef DEBUG
                printf(DEBUG_PREFIX"%s uninstalled.\n", APPLIST[i].prettyname);
                fflush(stdout);
#endif 
                retval = app_id;
            } else {
#ifdef DEBUG
                printf(DEBUG_PREFIX"%s could not uninstalled.\n", APPLIST[i].prettyname);
                fflush(stdout);
#endif 
                retval = -1;
            }
            
            if (manifest_filepath)
                free(manifest_filepath);
            return retval;
        }
    }

    return -1;
}

int get_applist() {
    DIR *d;
    struct dirent *entry;
    FILE *file;
    long size;
    char *filecontent;
    int ret;

    ret = chdir(MANIFEST_DIR);
    if (ret != 0) {
        handle_error("chdir");
    }

    d = opendir(MANIFEST_DIR);

    if (!d) {
        handle_error("opendir");
    }

    while ((entry = readdir(d)) && 
            number_of_applications < MAX_NUMBER_APPLICATIONS) {

        if (!strcmp (entry->d_name, "."))
            continue;
        if (!strcmp (entry->d_name, ".."))
            continue;
    
        file = fopen(entry->d_name, "r");
        if (!file) {
            printf(DEBUG_PREFIX"fopen error %s.\n", entry->d_name);
            continue;
        }

        fseek(file, 0, SEEK_END);
        size = ftell(file);
        rewind(file);
        filecontent = calloc(1, size+1);
        if (!filecontent) {
            fclose(file);
            printf(DEBUG_PREFIX"calloc error.\n");
            continue;
        }

        if (size != fread(filecontent, 1, size, file)) {
            free(filecontent);
            fclose(file);
            printf(DEBUG_PREFIX"fread error.\n");
            continue;
        }
        
        if (json_to_application(filecontent, number_of_applications) == 0)
            number_of_applications++;
    
        free(filecontent);
        fclose(file);
    }
    
    ret = add_system_apps(number_of_applications);
    if (ret > 0)
        number_of_applications += ret;

    /* Close the directory. */
    if (closedir (d)) {
        printf(DEBUG_PREFIX"closedir error.\n");
    }

    return 0;
}

int runapp (int appid) {
    int i, appindex = -1, ret;

    if (number_of_live_applications >= MAX_NUMBER_LIVE_APPLICATIONS) {
#ifdef DEBUG
        printf(DEBUG_PREFIX"Reached permitted number of live apps.\n");
        fflush(stdout);
#endif
        return -1;
    }

    /* Search for the appid in APPLIST. */
    for (i = 0; i < MAX_NUMBER_APPLICATIONS; i++) {
        if (APPLIST[i].id == appid) {
            appindex = i;
            break;
        }
    }
    
    if (appindex == -1) {
#ifdef DEBUG
        printf(DEBUG_PREFIX"Application id (%d) does not exists.\n", appid);
        fflush(stdout);
#endif
        return -2;
    }

    // FIX: Temporarily, do not integrity check for Soberstore
    if (appid != SOBERSTORE_APP_ID)
        if (!application_integrity_check(appindex)) {
            printf(DEBUG_PREFIX"Integrity check failed.\n");    
            return -3;
        }

    signal_sender(view_pid, /*SIGSTOP*/SIGKILL, VIEW);
    
    pid_t pid = run(APPLIST[appindex].path, APPLIST[appindex].name);
    ret = assign_control_group(pid, APPLIST[appindex].group);
#ifdef DEBUG
    printf(DEBUG_PREFIX"Cgroup assignment(%s)->%s: %d.\n", 
            APPLIST[appindex].prettyname, APPLIST[appindex].group, ret);
    fflush(stdout);
#endif  

    APPLIST[appindex].pid = pid;
    LIVEAPPS[number_of_live_applications] = &APPLIST[appindex];
    number_of_live_applications++;

    return 0;
}

void startapp (DBusMessage* msg, DBusConnection* conn) {
    DBusMessage* reply;
    DBusMessageIter args;
    dbus_uint32_t serial = 0;
    int rc = -1;
    dbus_uint32_t app_id;
    
    /* Read the arguments. */
    if (!dbus_message_iter_init(msg, &args)) {
#ifdef DEBUG
        fprintf(stderr, DEBUG_PREFIX"dbus: message has no arguments.\n");
        fflush(stderr);
#endif
    }
    else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) {
#ifdef DEBUG
        fprintf(stderr, DEBUG_PREFIX"dbus: argument is not integer.\n");
        fflush(stderr);
#endif
    }
    else {
        dbus_message_iter_get_basic(&args, &app_id);
        
        /* Run the corresponding application */
        rc = runapp(app_id);
        if (rc == 0) {
            /* Signal the condition variable that application has executed. */
            pthread_mutex_lock(&mutex);
            pthread_cond_signal(&child_exists);
            pthread_mutex_unlock(&mutex);
        }
    }

    /* Create a reply from the message. */
    reply = dbus_message_new_method_return(msg);

    /* Add the arguments to the reply. */
    dbus_message_iter_init_append(reply, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &rc)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: out of memory.\n");
    }

    /* Send the reply && flush the connection. */
    if (!dbus_connection_send(conn, reply, &serial)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: send error.\n");
    }

    dbus_connection_flush(conn);
    dbus_message_unref(reply);
}

void listapps (DBusMessage* msg, DBusConnection* conn) {
    DBusMessage* reply;
    DBusMessageIter args, struct_i, array_i;
    dbus_uint32_t serial = 0;
    unsigned int i = 0;

    /* Create a reply from the message. */
    reply = dbus_message_new_method_return(msg);

    /* Add the arguments to the reply. */
    dbus_message_iter_init_append(reply, &args);
    
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, 
                                        &number_of_applications)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: out of memory.\n");
    }

    dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "(uss)", &array_i);
    for (i = 0; i < number_of_applications; ++i) {
        dbus_message_iter_open_container(&array_i, DBUS_TYPE_STRUCT, NULL, &struct_i);
        
        dbus_message_iter_append_basic(&struct_i, DBUS_TYPE_UINT32, 
                                       &(APPLIST[i].id));
        dbus_message_iter_append_basic(&struct_i, DBUS_TYPE_STRING, 
                                       &(APPLIST[i].prettyname));
        dbus_message_iter_append_basic(&struct_i, DBUS_TYPE_STRING, 
                                       &(APPLIST[i].icon));
        
        dbus_message_iter_close_container(&array_i, &struct_i);

#ifdef DEBUG
        printf(DEBUG_PREFIX"%s(%d) supplied.\n", APPLIST[i].prettyname, APPLIST[i].id);
        fflush(stdout);
#endif
    }
    dbus_message_iter_close_container(&args, &array_i);

    /* Send the reply && flush the connection. */
    if (!dbus_connection_send(conn, reply, &serial)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: send error.\n");
    }

    dbus_connection_flush(conn);
    dbus_message_unref(reply);
}

void uninstallapps(DBusMessage* msg, DBusConnection* conn) {
    DBusMessage* reply;
    DBusMessageIter args, arrayIter;
    dbus_uint32_t serial = 0;
    int number_of_apps_to_remove = 0;
    int number_of_apps_removed = 0;
    int app_ids_to_remove[MAX_NUMBER_APPLICATIONS];
    int app_ids_removed[MAX_NUMBER_APPLICATIONS];
    int i, ret;
    
    /* 
     * Read the arguments.
     * number of applications to remove, and array of application ids
     * for applications.
     */
    if (!dbus_message_iter_init(msg, &args)) {
#ifdef DEBUG
        fprintf(stderr, DEBUG_PREFIX"dbus: message has no arguments.\n");
        fflush(stderr);
#endif   
    } else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) {
#ifdef DEBUG
        fprintf(stderr, DEBUG_PREFIX"dbus: argument is not integer.\n");
        fflush(stderr);
#endif    
    } else
        dbus_message_iter_get_basic(&args, &number_of_apps_to_remove);
    
    
    if (!dbus_message_iter_next(&args)) {
#ifdef DEBUG
        fprintf(stderr, DEBUG_PREFIX"dbus: message has no arguments.\n");
        fflush(stderr);
#endif  
    } else if (DBUS_TYPE_ARRAY != dbus_message_iter_get_arg_type(&args)) {
#ifdef DEBUG
        fprintf(stderr, DEBUG_PREFIX"dbus: argument is not array.\n");
        fflush(stderr);
#endif
    } else {
        dbus_message_iter_recurse (&args, &arrayIter);
        
        for (i = 0; i < number_of_apps_to_remove; ++i) {
            if (DBUS_TYPE_UINT32 == dbus_message_iter_get_arg_type(&arrayIter)) {
                dbus_message_iter_get_basic(&arrayIter, app_ids_to_remove+i);
            }
        
            dbus_message_iter_next(&arrayIter);
        }
    }
#ifdef DEBUG
    printf(DEBUG_PREFIX"Removing %d applications: ", number_of_apps_to_remove);
    for (i = 0; i < number_of_apps_to_remove; ++i) {
        printf("%d ", app_ids_to_remove[i]);
    }
    printf("\n");
    fflush(stdout);
#endif

    /* Remove applications. */
    for (i = 0; i < number_of_apps_to_remove; ++i) {
        ret = removeapp(app_ids_to_remove[i]);
        
        if (ret == app_ids_to_remove[i]) {
            app_ids_removed[number_of_apps_removed++] = ret;
        } else if (ret == -1) {
            printf(DEBUG_PREFIX"Could not remove application %d.\n",
                   app_ids_to_remove[i]);
        } else {
            printf(DEBUG_PREFIX"Removed wrong application: %d.\n", ret);
        }
    }
    
    if (number_of_apps_removed != 0) {
        number_of_applications = 0;
        if (get_applist()) {
            handle_error("get_applist");
        }
    }
    
#ifdef DEBUG
    printf(DEBUG_PREFIX"Removed %d applications: ", number_of_apps_removed);
    for (i = 0; i < number_of_apps_removed; ++i) {
        printf("%d ", app_ids_removed[i]);
    }
    printf("\n");
    fflush(stdout);
#endif

    /* Create a reply. */
    reply = dbus_message_new_method_return(msg);
    dbus_message_iter_init_append(reply, &args);
    
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32,
                                        &number_of_apps_removed)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: out of memory.\n");
    }
    
    dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "u", &arrayIter);

    dbus_int32_t *v_ARRAY = app_ids_removed;
    if (!dbus_message_iter_append_fixed_array (&arrayIter, DBUS_TYPE_UINT32, &v_ARRAY, number_of_apps_removed)) {
        fprintf (stderr, "No memory!\n");
    }
    
    dbus_message_iter_close_container(&args, &arrayIter);
    
    /* Send the reply. */
    if (!dbus_connection_send(conn, reply, &serial)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: send error.\n");
    }
    
    dbus_connection_flush(conn);
    dbus_message_unref(reply);
}

void login(DBusMessage* msg) {
    DBusMessageIter args;
    dbus_uint32_t access_code;
    
    /* Read the arguments. */
    if (!dbus_message_iter_init(msg, &args)) {
#ifdef DEBUG
        fprintf(stderr, DEBUG_PREFIX"dbus: message has no arguments.\n");
        fflush(stderr);
#endif
    }
    else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) {
#ifdef DEBUG
        fprintf(stderr, DEBUG_PREFIX"dbus: argument is not integer.\n");
        fflush(stderr);
#endif
    }
    else {
        dbus_message_iter_get_basic(&args, &access_code);
        
        if (access_code == 0) {
            signal_sender(login_pid, SIGSTOP, LOGIN);
            signal_sender(view_pid, SIGCONT, VIEW);
        }
    }
}

void updateapps () {
    if (get_applist()) {
        handle_error("get_applist");
    }
}

void lockscreen() {
    signal_sender(view_pid, SIGSTOP, VIEW);
    signal_sender(login_pid, SIGCONT, LOGIN);
}

void listen() {
    DBusMessage* msg;
    DBusConnection* conn;
    DBusError err;
    int ret;

    /* Initialize error. */
    dbus_error_init(&err);

    /* Connect to the bus and check for errors. */
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);

    if (dbus_error_is_set(&err)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: connection error (%s).\n", err.message);
        dbus_error_free(&err);
        handle_error("listen");
    }

    if (NULL == conn) {
        fprintf(stderr, DEBUG_PREFIX"dbus: null connection.\n");
        handle_error("listen");
    }

    /* Request our name on the bus and check for errors. */
    ret = dbus_bus_request_name(conn, "appman.method.server",
                                DBUS_NAME_FLAG_REPLACE_EXISTING , &err);

    if (dbus_error_is_set(&err)) {
        fprintf(stderr, DEBUG_PREFIX"dbus: name error (%s).\n", err.message);
        dbus_error_free(&err);
        handle_error("listen");
    }

    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
        fprintf(stderr, DEBUG_PREFIX"dbus: not primary owner (%d).\n", ret);
        handle_error("listen");
    }
    
    while (1) {
        /* Non-blocking read of the next available message. */
        dbus_connection_read_write(conn, 0);
        msg = dbus_connection_pop_message(conn);

        if (msg == NULL) {
            sleep(1);
            continue;
        }
#ifdef DEBUG
        printf(DEBUG_PREFIX"Coming dbus message:\n");
        printf("\tType: %d\n", dbus_message_get_type(msg));
        printf("\tMember: %s\n", dbus_message_get_member(msg));
        printf("\tSignature: %s\n", dbus_message_get_signature(msg));
        printf("\tSender: %s\n", dbus_message_get_sender(msg));
        printf("\tPath: %s\n", dbus_message_get_path(msg));
        printf("\tInterface: %s\n", dbus_message_get_interface(msg));
        printf("\tDestination: %s\n", dbus_message_get_destination(msg));
        fflush(stdout);
#endif
        
        /* Check this is a method call for the right interface & method. */
        if (dbus_message_is_method_call(msg, "appman.method.Type", "startapp")) {
            startapp(msg, conn);
        } 
        else if (dbus_message_is_method_call(msg, "appman.method.Type", "listapps")) {
            listapps(msg, conn);
        } 
        else if (dbus_message_is_method_call(msg, "appman.method.Type", "login")) {
            login(msg);
        } 
        else if (dbus_message_is_method_call(msg, "appman.method.Type", "updateapps")) {
            updateapps();
        } 
        else if (dbus_message_is_method_call(msg, "appman.method.Type", "lockscreen")) {
            lockscreen();
        } 
        else if (dbus_message_is_method_call(msg, "appman.method.Type", "uninstallapps")) {
            uninstallapps(msg, conn);
        }
#ifdef DEBUG
        else {
            DBusMessageIter args;
            char* sigvalue;
            dbus_message_iter_init(msg, &args);
            dbus_message_iter_get_basic(&args, &sigvalue);
            printf(DEBUG_PREFIX"dbus: unexpected message: %s.\n", sigvalue);
        }
#endif

        /* Free the message. */
        dbus_message_unref(msg);
    }

    /* Close the connection. */
    dbus_connection_close(conn);
    dbus_connection_unref(conn);
}

void *request_handler(void *targs) {
    sigset_t set;
    int s;
    // thread_info *tinfo = targs;

    /* Mask signal handling for threads other than main thread. */
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    s = pthread_sigmask(SIG_BLOCK, &set, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_sigmask");

    /* Listen and answer D-Bus requests to run applications. */
    listen();

    pthread_exit(NULL);
}

void status_handler(int pid, int status) {
    int i;
    
    /* Child terminated normally. */
    if (WIFEXITED(status)) {
#ifdef DEBUG
        printf(DEBUG_PREFIX"pid:%d terminated normally.\n", pid);
        fflush(stdout);
#endif
        
    }
    /* Child terminated by signal. */
    else if (WIFSIGNALED(status)) {
#ifdef DEBUG
        printf(DEBUG_PREFIX"pid:%d terminated by signal.\n", pid);
        fflush(stdout);
#endif      
    }
    /* Child produced a core dump. */
    else if (WCOREDUMP(status)) {
#ifdef DEBUG
        printf(DEBUG_PREFIX"pid:%d produced a core dump.\n", pid);
        fflush(stdout);
#endif
    }
    /* Child stopped by signal. */
    else if (WIFSTOPPED(status)) {
#ifdef DEBUG
        printf(DEBUG_PREFIX"pid:%d stopped by signal.\n", pid);
        fflush(stdout);
#endif
    }
    /* Other conditions. */
    else {
#ifdef DEBUG
        printf(DEBUG_PREFIX"pid:%d anything then handled reasons.\n", pid);
        fflush(stdout);
#endif
    }
    
    if (pid == view_pid) {
        /* Then restart appman view. */
        //view_pid = run(system_apps[VIEW][0], system_apps[VIEW][1]);
    } else if (pid == login_pid) {  
        /* Then restart appman login. */
        login_pid = run(system_apps[LOGIN][0], system_apps[LOGIN][1]);
    } else {
        for (i = 0; i < number_of_live_applications; i++) {
            /* Find which running app has changed its states. */
            if (pid == LIVEAPPS[i]->pid) {
#ifdef DEBUG
                printf(DEBUG_PREFIX"%s is going to shutdown.\n", LIVEAPPS[i]->prettyname);
                fflush(stdout);
#endif
                LIVEAPPS[i] = NULL;
                number_of_live_applications--;
                break;
            }
        }
        signal_sender(view_pid, SIGCONT, VIEW);
    }
}
    
int signal_sender (int pid, int signo, int app) {
    int ret = 0;
    
    /* Check existence of process id. */
    if (pid == 0) {
        if (signo == SIGCONT) {
            /* If not exists, restart. */
            if (app == LOGIN)
                login_pid = run(system_apps[app][0], system_apps[app][1]);
            else if (app == VIEW)
                view_pid = run(system_apps[app][0], system_apps[app][1]);
        }
    } else {
        ret = kill(pid, 0);
        if (ret == -1 && errno == ESRCH) {
            printf(DEBUG_PREFIX"pid: %d not exists.\n", pid);
            fflush(stdout);
            
            if (signo == SIGCONT) {
                /* If not exists, restart. */
                if (app == LOGIN)
                    login_pid = run(system_apps[app][0], system_apps[app][1]);
                else if (app == VIEW)
                    view_pid = run(system_apps[app][0], system_apps[app][1]);
            }
            
        } else {
            /* If exists, send signal. */
            ret = kill(pid, signo);
            if (ret != 0) {
                printf(DEBUG_PREFIX"signal(%d) could not send pid:%d.\n", signo, view_pid);
                fflush(stdout);
            }            
        }
    }
    
    return ret;
}

void signal_handler(int signo, siginfo_t *info, void *p) {
    int status;
    int rc;

#ifdef DEBUG
    printf(DEBUG_PREFIX"Signal %d received. "
            "si_errno %d, "    /* An errno value */
            "si_code %s.\n",     /* Signal code */
            signo,
            info->si_errno,
            reasonstr(signo, info->si_code));
#endif

    if (signo == SIGCHLD) {
#ifdef DEBUG
        printf(DEBUG_PREFIX);
        printf( "si_pid %d, "       /* Sending process ID */
                "si_uid %d, "       /* Real user ID of sending process */
                "si_status %d, "    /* Exit value or signal */
                "si_utime %ld, "    /* User time consumed */
                "si_stime %ld.\n",  /* System time consumed */
                info->si_pid,
                info->si_uid,
                info->si_status,
                info->si_utime,
                info->si_stime);
        fflush(stdout);
#endif

        /*
         * Multiple child processes could terminate
         * while one is in the process being reaped.
         * Loop ensures that any zombies which existed
         * prior to invocation of the handler function
         * will be reaped.
         */
        while ((rc = waitpid((pid_t)(-1), &status, WNOHANG)) > 0) {
            status_handler(rc, status);
        }
    }
}

int main (int argc, char *argv[]) {
    thread_info *tinfo;
    int rc;
    int i;
    void *res;
    struct sigaction act;
    int status;

    /* Memory allocation for thread arguments */
    tinfo = malloc(sizeof(thread_info) * NUMBER_OF_THREADS);
    if (!tinfo)
        handle_error("malloc for thread_info");

    /* Fill application list. */
    rc = get_applist();
    if (rc) {
        handle_error("get_applist");
    }

    /* Register signal handler. */
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = signal_handler;
    sigaction(SIGCHLD, &act, NULL);

    /* Thread creation */
    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        rc = pthread_create(&tinfo[i].thread, NULL,
                            request_handler, (void*)&tinfo[i]);
        if (rc) {
            handle_error_en(rc, "pthread_create");
        }
    }
    /* Time for thread initializations. */
    sleep(1);
    
    /* Start the appman login. */
    login_pid = run(system_apps[LOGIN][0], system_apps[LOGIN][1]);
    
    /*
     * Infinite loop of process waits. If there are processes
     * to wait, wait any of them. If not, wait on the given condition
     * variable. When signaled, redo.
     */
    while (1) {
        rc = waitpid((pid_t)(-1), &status, 0);

        if (rc <= 0) {
            if (errno == ECHILD) {
                /* Does not have child, wait on condition. */
                rc = pthread_mutex_lock(&mutex);
                rc = pthread_cond_wait(&child_exists, &mutex);
                rc = pthread_mutex_unlock(&mutex);
                continue;
            } else if (errno == EINVAL) {
                handle_error_en(errno, "waitpid: invalid options");
            }

        } else {
            status_handler(rc, status);
        }
    }

    /* Join with threads */
    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        rc = pthread_join(tinfo[i].thread, &res);

        if (rc) {
            handle_error_en(rc, "pthread_join");
        }

        free(res);
    }

    free(tinfo);
    exit(EXIT_SUCCESS);
}
