#ifndef _APPMAND_H_
#define _APPMAND_H_

#define _GNU_SOURCE /* For recursive mutex initializer. */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <openssl/sha.h>
#include "cJSON.h"

#define NUMBER_OF_THREADS 1 /* Thread number to handle requests. */
#define MAX_NUMBER_APPLICATIONS 50 /* Maximum number of applications. */
#define MAX_NUMBER_LIVE_APPLICATIONS 1 /* Maximum number of running applications. */
#define MANIFEST_DIR "/etc/appmand/" /* Manifest storage. */
#define CGROUPS_VFS "/sys/fs/cgroup/" /* Cgroups virtual filesystem path. */
#define SOBERSTORE_APP_ID 0 /* Soberstore application id. */

#define DEBUG_PREFIX "appmand: "

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0);
#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0);

typedef enum { false, true } bool;
enum { LOGIN, VIEW, SOBERSTORE };

/*
 * Thread information.
 * Passed as an argument from main process to threads.
 */
typedef struct thread_info {
    pthread_t thread;
    
} thread_info;

/*
 * Application structure.
 * id           : application id.
 * path         : execution path.
 * name         : application binary name.
 * group        : cgroup group name.
 * prettyname   : pretty application name.
 * icon         : application icon.
 * hash         : sha256 binary hash.
 * pid          : process id of app.
 */
typedef struct application {
    unsigned int id;
    char* path;
    char* name;
    char* group;
    char* prettyname;
    char* icon;
    char* hash;
    pid_t pid; /* in case of demand, -1 until first run. */
} application;

/* String expression of signal code. */
const char *reasonstr(int, int);

/* Compares binary sha256 hash w/ manifest hash value. */
bool application_integrity_check(int);

/* Fork, exec and return pid of a child. */
pid_t run (const char *, const char *);

/* Assign process to group. Return 0 if success, -1 otherwise. */
int assign_control_group(pid_t, const char *);

/*
 * Convert json data to application structure.
 * Return 0 if success.
 */
int json_to_application(char *, int);

/**
 * Some applications should be served to UI even they don't have manifest. 
 * Add system apps and return added application number.
 * - Soberstore
 * - ..
 */
int add_system_apps (int);

/* Remove filepath. Returns 0 if success. */
int removefile (char* filepath);

/*
 * Remove application files from system. (i.e. manifest, binary)
 * Returns removed application id, -1 if unsuccessful.
 */
int removeapp(int app_id);

/*
 * Reads application list from manifest directory.
 * Returns 0 if success.
 */
int get_applist();

/*
 * Runs application with the given id.
 * Returns 0 if success.
 *        -1 if reached maximum number of live apps.
 *        -2 if application id does not exists.
 *        -3 if integrity check fails.
 */
int runapp(int);

/*
 * DBUS METHODS TO SERVE
 * ---------------------
 * startapp: Start application.
 * listapps: Return applications.
 * uninstallapps: Remove applications from system.
 * login: Bypass 'login' if 'success' access code received.
 * updateapps: Update application list.
 * lockscreen: Run 'login' application that asks pin.
 */
void startapp (DBusMessage*, DBusConnection*);
void listapps (DBusMessage*, DBusConnection*);
void uninstallapps(DBusMessage* msg, DBusConnection* conn);
void login (DBusMessage* msg);
void updateapps();
void lockscreen();

/* Expose a method call and wait for it to be called. */
void listen();

/* Request handling with dbus. */
void *request_handler(void *);

/* Send signal to process. */
int signal_sender (int, int, int);

/* Handle termination status of child process. */
void status_handler(int, int);

/* Signal handling for main process. */
void signal_handler(int, siginfo_t *, void *);

application APPLIST[MAX_NUMBER_APPLICATIONS]; /* Application list. */
unsigned int number_of_applications = 0; /* Number of applications. */
application *LIVEAPPS[MAX_NUMBER_LIVE_APPLICATIONS]; /* Running applications. */
unsigned int number_of_live_applications = 0; /* Number of running applications. */
pid_t login_pid = 0; /* Process id of a appman login. */
pid_t view_pid = 0; /* Process id of a appman view. */

#endif  /* not defined _APPMAND_H_ */
