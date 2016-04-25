#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "appsmodel.h"
#include <QVariant>
#include <QList>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


AppsModel appsModel;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("AppsModel", &appsModel);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}



