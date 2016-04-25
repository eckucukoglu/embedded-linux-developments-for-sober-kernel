TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    cpumemhandler.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    cpumemhandler.h

INCLUDEPATH +=   /usr/include/


DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS
