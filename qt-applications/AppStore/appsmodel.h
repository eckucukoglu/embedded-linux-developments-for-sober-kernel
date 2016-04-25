#ifndef APPSMODEL_H
#define APPSMODEL_H
#define MAX_NUMBER_APPLICATIONS 50
#define APPMAN_VIEW_DEBUG_PREFIX "V >> "
#define DBUS_API_SUBJECT_TO_CHANGE

#include <QObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>
#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "HTTPPerform.h"
#include <ctime>

using namespace std;
typedef struct app {
        int id;
        string name;
        string developerName;
        string iconName;
        string hashValue;
        string binaryPath;
        string binaryName;
        string packagePath;
        int isDownloaded;
        int isInstalled;
        int error;
        string errorCode;
        double version;
        int date;
        string borderColor;
}app;

typedef struct installed_app {
    unsigned int id;
    char* prettyname;
    char* icon;
} installed_app;


class AppsModel: public QObject
{
    Q_OBJECT
public:
    explicit AppsModel(QObject *parent = 0);
    Q_INVOKABLE QVariant get_element_list();
    Q_INVOKABLE void set_element_list(app list[]);
    Q_INVOKABLE int get_current_index();
    Q_INVOKABLE void set_current_index(int index);
    Q_INVOKABLE int get_page_index();
    Q_INVOKABLE void set_page_index(int index);
    Q_INVOKABLE int get_number_of_applications();
    Q_INVOKABLE int get_applist();
    Q_INVOKABLE void set_page_count();
    Q_INVOKABLE int get_page_count();
    Q_INVOKABLE int download(int appid);
    Q_INVOKABLE void listApps();
    Q_INVOKABLE void query_listapps();
    Q_INVOKABLE int check_error();
    Q_INVOKABLE int check_if_installed(int app_id);
    Q_INVOKABLE void assert_dbus_method_return (DBusMessage* msg);
    int number_of_applications;
    int number_of_installed_applications;
    Q_INVOKABLE bool check_connection();

 private:
     int current_index;
     int page_index;
     int page_count;
     int error;
     bool check_internet;
     app APPLIST[MAX_NUMBER_APPLICATIONS];
     HTTPPerform* performer;
     applications* _appList;
     Q_INVOKABLE QVariant appList;
     installed_app INSTALLEDAPPS[MAX_NUMBER_APPLICATIONS];
};

#endif // APPSMODEL_H
