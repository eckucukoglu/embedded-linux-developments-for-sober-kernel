QT += qml quick widgets
QML_IMPORT_PATH = /usr/include/qt5/QtQml/

HEADERS       = button.h \
                calculator.h
SOURCES       = button.cpp \
                calculator.cpp \
                main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/calculator
INSTALLS += target

RESOURCES += qml.qrc

