#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "loginhelper.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    LoginHelper loginHelper;
    engine.rootContext()->setContextProperty("loginHelper", &loginHelper);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
