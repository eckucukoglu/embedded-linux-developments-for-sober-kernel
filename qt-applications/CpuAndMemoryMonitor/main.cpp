#include <QApplication>
#include <QQmlApplicationEngine>
#include "cpumemhandler.h"
#include <QQmlEngine>
#include <QtQml>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qmlRegisterType<CpuMemHandler, 1>("CpuMemHandler", 1, 0, "CpuMemHandler");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
