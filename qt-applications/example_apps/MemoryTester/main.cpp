#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "helper.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Helper helper;
    engine.rootContext()->setContextProperty("helper", &helper);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
