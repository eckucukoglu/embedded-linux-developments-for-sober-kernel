#include <string.h>
#include "cJSON.h"
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <openssl/sha.h>
#include <dbus/dbus.h>

using namespace std;


enum ACTION{
    INDEX,
    SHOW,
    DOWNLOAD,
    GETSHARE,
    SENDSHARE
};
struct application {
        int id;
        string name;
        string developerName;
        string iconName;
        string hashValue;
        string binaryPath;
        string binaryName;
        string packagePath;
        string cgroup;
        int isDownloaded;
        int isInstalled;
        int error;
        int size;
        string errorCode;
};
struct applications {
    int size;
    application* apps;
};

class HTTPPerform {

public:
    HTTPPerform(string);
    ~HTTPPerform();
    applications* perform(ACTION, int);
    void perform(ACTION action, string &response);
    int getError();
    string getUrl();
    string getErrorMessage();

private:
    string errorMessage;
    string baseUrl;
    applications* parseString(string);
    int parseShamirShare(string strJsonToParse,string &share);
    void* curl;
    int errorFlag;
    void setUrl(const string&);
    int install(const string&, application*);
    int download(const string&, application*);
    int getContent(string,string&);
    int sendMessage(string url, string msgToSend, string& content);
};
