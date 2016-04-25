#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <QObject>
#include <QtNetwork>

class NetworkHandler : public QObject
{
    Q_OBJECT
public:
    explicit NetworkHandler(QObject *parent = 0);

    Q_INVOKABLE QString getMACAddress(QString ifaceName);
    Q_INVOKABLE QString getIP(QString ifaceName);
    Q_INVOKABLE QString getNetmask(QString ifaceName);
    Q_INVOKABLE QString getGateway(QString ip, QString netmask);

    Q_INVOKABLE bool checkConnection(QString ifaceName);

    Q_INVOKABLE void applyConfiguration(QString ifaceName, QString autoOrManual, QString ip, QString netmask, QString gateway, QString dns);

    void addDNS(QString interfaceName, QString dns);
    void resetDNSSettings(QString interfaceName);
    void updateIfstateFile(QString interfaceName);

    void addCfgLineToInterfaces(QString interfaceName);
    bool deleteInterfacesConfigFile(QString interfaceName);
    void prepareInterfacesConfigFile(QString interfaceName, QString autoOrManual, QString ip, QString netmask, QString gateway);

    void ifDownIfUp(QString interfaceName);
signals:

public slots:

private:


};

#endif // NETWORKHANDLER_H
