# appmand: Application manager daemon

appmand is a daemon to control application spesific jobs for linux distributions. appmand lists system applications, runs them, signals them to stop or continue. Uses cgroups to assign them to arranged groups. Moreover appmand does binary hash checks with sha256 algorithm to validate binary integrity.

System has mandatory access control rules to protect **application manifests**, which can only be accessible by appmand. So that other applications that want to list, run or access applications have to request corresponding d-bus methods. appmand has 2 additional ui applications: **appmand-login** and **appmand-view**. Former is used to lock screen, and latter is a basic launcher.

appmand serves various D-Bus methods:
  - startapp: Execute application.
  - listapps: Return application list.
  - uninstallapps: Remove applications from system.
  - login: Bypass **appmand-login** if 'success' access code received.
  - updateapps: Update application list.
  - lockscreen: Switch **appmand-login** application that asks pin.

Currently, maximum number for live applications is only 1, it is a restriction for a spesific embedded linux project. It means that in case of any UI (Qt) application end, **appmand-view** will be run by appmand. 

### Manifest files:
Manifests are basically define the application. Default manifest path is **/etc/appmand/**. Manifest files ({application-id}.mf) are in json format and each application needs its own manifest file. Applications have a unique unsigned integer value as **id**.
```
{
	"id":   115,
	"path": "/usr/bin/gnome-calculator",
	"name": "gnome-calculator",
	"group":    "none",
	"prettyname":   "Calculator",
	"icon": "Calculator.png",
	"hash": "40bfb3b6565739ef668ff4d33e9cabd3f107a21d37d97f82646145bec5a89641"
}
```

### Application structure:
 * id           : application id.
 * path         : execution path.
 * name         : application binary name.
 * group        : control group name.
 * prettyname   : pretty application name.
 * icon         : application icon.
 * hash         : sha256 binary hash.
 * pid          : process id of app.

### Compilation:
Makefile has 3 targets. These libraries are needed for successful compilation: -lm -lpthread -lcrypto -ldbus-1
 * default: compiles default **appmand**.
 * debug: compiles **appmand** which has lots of debug outputs.
 * tester: compiles **tester** for d-bus method queries.
```
make debug
make tester
```

### Usage:
Since appmand is a basic daemon, good to run it as a background application:
```
./appmand &
```
To test d-bus methods, basic tester added. You can use it like:
```
./tester [startapp|listapps|uninstallapps|login|updateapps|lockscreen] [<param>]
```

### External libraries: 
appmand uses a number of open source projects to work properly:
  - libdbus-1-dev is required for dbus functionality.
  - [cJSON] is used as a Json parser.
  - openssl is required for sha256 hash integrity checks.
  - qt-quick framework is used to implement **appmand-view** and **appmand-login** applications.
  - **appmand-view** and **appmand-login** can be accessible from [here][qt-apps], respectively DesktopScreen and LoginScreen.

### License
GNU GENERAL PUBLIC LICENSE Version 2


[cJSON]: <https://sourceforge.net/projects/cjson/>
[qt-apps]: <https://github.com/eckucukoglu/qt-applications>

