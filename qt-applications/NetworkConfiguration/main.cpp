#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QtQml>
#include "networkhandler.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<NetworkHandler, 1>("NetworkHandler", 1, 0, "NetworkHandler");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
