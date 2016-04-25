#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QtQml>
#include "selinuxhandler.h"
#include "encdechandler.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<SELinuxHandler, 1>("SELinuxHandler", 1, 0, "SELinuxHandler");
    qmlRegisterType<EncDecHandler, 1>("EncDecHandler", 1, 0, "EncDecHandler");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
