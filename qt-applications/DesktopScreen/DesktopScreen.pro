TEMPLATE = app

QT += qml quick widgets


QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    appsmodel.cpp \
    security.cpp \
    HTTPPerform.cpp \
    cJSON.cpp

RESOURCES += qml.qrc

#Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = /usr/include/qt5/QtQml/

# Default rules for deployment.
include(deployment.pri)


######################## added for C
INCLUDEPATH += /usr/include/dbus-1.0 \
               /usr/include/

LIBS += -L/usr/include/dbus-1.0 \
        -L/home/arcelik/MMIS_ARGE/filesystems/5/lib \
        -ldbus-1 -lcurl -lcrypto -lssl -lpthread -largon2



HEADERS += \
    appsmodel.h \
    security.h \
    argon2.h \
    HTTPPerform.h \
    cJSON.h

CONFIG += dbus



DISTFILES += \
    pics/sober_newspecs/icon/icon_calendar.png \
    Calculator.png \
    Sci.png
