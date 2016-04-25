#include "networkhandler.h"

NetworkHandler::NetworkHandler(QObject *parent) : QObject(parent)
{
    QNetworkInterface interfaces;
    QList<QNetworkInterface> list = interfaces.allInterfaces();
}

QString NetworkHandler::getMACAddress(QString ifaceName){
    QNetworkInterface interfaces;
    QList<QNetworkInterface> list = interfaces.allInterfaces();

    QString interfaceName = (ifaceName.toLower() == "ethernet") ? "eth0" : ((ifaceName.toLower() == "wireless") ? "wlan0" : "invalid");

    if(interfaceName != "invalid"){
        foreach(QNetworkInterface i, list){
            if(i.name().startsWith(interfaceName))
                return i.hardwareAddress();
        }
    }
    return "HH:HH:HH:HH:HH:HH";
}

QString NetworkHandler::getIP(QString ifaceName){
    QNetworkInterface interfaces;
    QList<QNetworkInterface> list = interfaces.allInterfaces();

    QString interfaceName = (ifaceName.toLower() == "ethernet") ? "eth0" : ((ifaceName.toLower() == "wireless") ? "wlan0" : "invalid");

    if(interfaceName != "invalid"){
        foreach(QNetworkInterface i, list){
            if(i.name().startsWith(interfaceName)){
                QList<QNetworkAddressEntry> entries = i.addressEntries();
                if(!entries.isEmpty()){
                    QNetworkAddressEntry entry = entries.first();
                    QHostAddress address = entry.ip();
                    if(address.protocol() == QAbstractSocket::IPv4Protocol)
                        return entry.ip().toString();
                }
            }
        }
    }
    return "0.0.0.0";
}

QString NetworkHandler::getNetmask(QString ifaceName){
    QNetworkInterface interfaces;
    QList<QNetworkInterface> list = interfaces.allInterfaces();

    QString interfaceName = (ifaceName.toLower() == "ethernet") ? "eth0" : ((ifaceName.toLower() == "wireless") ? "wlan0" : "invalid");

    if(interfaceName != "invalid"){
        foreach(QNetworkInterface i, list){
            if(i.name().startsWith(interfaceName)){
                QList<QNetworkAddressEntry> entries = i.addressEntries();
                if(!entries.isEmpty()){
                    QNetworkAddressEntry entry = entries.first();
                    QHostAddress address = entry.netmask();
                    if(address.protocol() == QAbstractSocket::IPv4Protocol)
                        return entry.netmask().toString();
                }
            }
        }
    }
    return "255.255.255.255";
}

QString NetworkHandler::getGateway(QString ip, QString netmask){
    QNetworkInterface interfaces;
    QList<QNetworkInterface> list = interfaces.allInterfaces();


    if(ip == "0.0.0.0")
        return "0.0.0.0";

    QString gateway;
    QProcess process;
    QString getGatewayCommand = "netstat -rn | grep 0.0.0.0 | awk '{print $2}' | grep -v '0.0.0.0'";

    process.start("bash", QStringList() << "-c" << getGatewayCommand);
    process.waitForFinished();
    QString gatewayCandidate = process.readAllStandardOutput();
    if(gatewayCandidate.isEmpty())
        gateway = "0.0.0.0";
    else{
        gatewayCandidate.chop(1);
        //QHostAddress gate(gatewayCandidate);

        //if(gate.isInSubnet(QHostAddress::parseSubnet(ip + "/" + netmask)))
            gateway = gatewayCandidate;
//        else{
//            gateway = "0.0.0.0";
//        }
    }
    return gateway;

}

bool NetworkHandler::checkConnection(QString ifaceName){
    QNetworkInterface interfaces;
    QList<QNetworkInterface> list = interfaces.allInterfaces();

    QString interfaceName = (ifaceName.toLower() == "ethernet") ? "eth0" : ((ifaceName.toLower() == "wireless") ? "wlan0" : "invalid");

    if(interfaceName != "invalid"){
        foreach(QNetworkInterface i, list){
            if(i.name().startsWith(interfaceName)){
                return i.flags().testFlag(QNetworkInterface::IsUp) && i.flags().testFlag(QNetworkInterface::IsRunning);
            }
        }
    }
    return false;
}

void NetworkHandler::applyConfiguration(QString ifaceName, QString autoOrManual, QString ip, QString netmask, QString gateway, QString dns){
    QString interfaceName = (ifaceName.toLower() == "ethernet") ? "eth0" : ((ifaceName.toLower() == "wireless") ? "wlan0" : "invalid");

    addCfgLineToInterfaces(interfaceName);
    if(autoOrManual.toLower() == "auto"){
        prepareInterfacesConfigFile(interfaceName, autoOrManual, ip, netmask, gateway);
        resetDNSSettings(interfaceName);
    }
    else if (autoOrManual.toLower() == "manual"){
        prepareInterfacesConfigFile(interfaceName,autoOrManual, ip, netmask, gateway);

        if(!dns.isEmpty())
            addDNS(interfaceName, dns);
    }

    ifDownIfUp(interfaceName);

}

void NetworkHandler::addDNS(QString interfaceName, QString dns){

    QString resolvconfHeadpath = "/etc/resolvconf/resolv.conf.d/";
    //***********************************************************
    //Add the dns to /etc/resolvconf/resolv.conf.d/head [empty dns config when auto selected]
    //Creating the head file if there is not any.
    //***********************************************************
    QDir resolvConf = QDir::root();
    resolvConf.mkpath(resolvconfHeadpath);
    resolvConf.cd(resolvconfHeadpath);

    //Now directory is in resolv.conf.d/
    QFile resolvConfHead(resolvConf.filePath("head"));
    //open the file
    if(!resolvConfHead.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)){ //each time we write the head from scratch
        qDebug() << "Cannot create/open the file " + resolvConfHead.fileName();
        qDebug() << resolvConfHead.errorString();
    }

    //Preparing the dns string and adding it to the head file
    QString resolvConfHeadContents;
    resolvConfHeadContents = "nameserver " + dns + "\n";

    //Add the contents to the config file
    QTextStream resolvConfHeadFileIn(&resolvConfHead);
    resolvConfHeadFileIn << resolvConfHeadContents;
    resolvConfHead.close();

    updateIfstateFile(interfaceName);


    //***********************************************************
    //Reset the resolv.conf content
    //***********************************************************
    QString rccommand = "resolvconf -u";
    QProcess process;
    process.execute("bash", QStringList() << "-c" << rccommand);

}

void NetworkHandler::updateIfstateFile(QString interfaceName){
    QString ifstatePath = "/run/network/ifstate";

    //***********************************************************
    //Add the connectionType=connectionType line into the ifstate.
    //***********************************************************

    QFile ifstateFile(ifstatePath);

    //open the file
    if(!ifstateFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Cannot create the file " + ifstateFile.fileName();
        qDebug() << ifstateFile.errorString();
    }

    //check if there is 'connectionType=connectionType' in the file.
    //if there isnt any, add it.
    QTextStream ifstateStream(&ifstateFile);
    QString ifstateContents = ifstateStream.readAll();
    if(!ifstateContents.contains("" + interfaceName + "=" + interfaceName))
        ifstateStream << interfaceName + "=" + interfaceName;

    ifstateFile.close();
}

void NetworkHandler::resetDNSSettings(QString interfaceName){
    QString resolvconfHeadpath = "/etc/resolvconf/resolv.conf.d";
    QDir resolvConf = QDir::root();
    resolvConf.mkpath(resolvconfHeadpath);
    resolvConf.cd(resolvconfHeadpath);

    //Now directory is in resolv.conf.d/
    QFile resolvConfHead(resolvConf.filePath("head"));
    //open the file
    if(!resolvConfHead.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)){ //each time we write the head from scratch
        qDebug() << "Cannot create/open the file " + resolvConfHead.fileName();
        qDebug() << resolvConfHead.errorString();
    }

    //Add empty character to the config file
    QTextStream resolvConfHeadFileIn(&resolvConfHead);
    resolvConfHeadFileIn << "";
    resolvConfHead.close();

    updateIfstateFile(interfaceName);

    //***********************************************************
    //Reset the resolv.conf content
    //***********************************************************
    QString rccommand = "resolvconf -u";
    QProcess process;
    process.execute("bash", QStringList() << "-c" << rccommand);
}

bool NetworkHandler::deleteInterfacesConfigFile(QString interfaceName){
    QDir configs = QDir::root();
    QString interfacesdotDpath = "/etc/network/interfaces.d/";
    configs.mkpath(interfacesdotDpath);
    configs.cd(interfacesdotDpath);

    //Now directory is in interfaces.d/
    QFile configfile(configs.filePath(interfaceName + ".cfg"));
    if(configfile.remove())
        return true;
    return false;
}

void NetworkHandler::prepareInterfacesConfigFile(QString interfaceName,QString autoOrManual, QString ip, QString netmask, QString gateway){

    //***********************************************************
    //Creating the config file:
    //***********************************************************
    QString interfacesdotDpath = "/etc/network/interfaces.d/";
    QDir configs = QDir::root();
    configs.mkpath(interfacesdotDpath);
    configs.cd(interfacesdotDpath);

    //Now directory is in interfaces.d/
    QFile configfile(configs.filePath(interfaceName + ".cfg"));

    if(!configfile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)){
        qDebug() << "Cannot create/open the file " + configfile.fileName();
        qDebug() << configfile.errorString();
    }

    //Preparing the cfg file to be added:
    QString configContents = "";
    if(autoOrManual.toLower() == "manual"){
        //configContents += "auto " + interfaceName + "\n";
        configContents += "iface " + interfaceName + " inet static\n";
        configContents += "\taddress " + ip + "\n";
        configContents += "\tnetmask " + netmask + "\n";
        configContents += "\tgateway " + gateway + "\n";

    }
    else if(autoOrManual.toLower() == "auto"){
        configContents += "auto " + interfaceName + "\n";
        configContents += "iface " + interfaceName + " inet dhcp";

    }

    //Adding the contents to the config file
    QTextStream configFileIn(&configfile);
    configFileIn << configContents;
    configfile.close();

}

void NetworkHandler::addCfgLineToInterfaces(QString interfaceName){

    QString interfacesdotDpath = "/etc/network/interfaces.d/";
    QDir configs = QDir::root();
    configs.mkpath(interfacesdotDpath);
    configs.cd(interfacesdotDpath);

    //Now directory is in interfaces.d/
    QFile configfile(configs.filePath(interfaceName + ".cfg"));


    //Open the etc/network/interfaces file and get the original content of the file:
    QString originalContentOfInterfaces;
    QFile interfaces("/etc/network/interfaces");
    if (!interfaces.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Could not open " + interfaces.fileName();
        qDebug() << interfaces.errorString();
    }
    else{
        originalContentOfInterfaces = "";
        QTextStream in(&interfaces);
        originalContentOfInterfaces = in.readAll();
        interfaces.close();
    }

    //***********************************************************
    //add the necessary config file's source
    //to interfaces file:
    //***********************************************************
    QTextStream writerToInterfacesFile(&interfaces);
    if (!interfaces.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)){
        qDebug() << "Could not open " + interfaces.fileName();
        qDebug() << interfaces.errorString();
    }

    //append the source or do nothing according to our inspection:
    if(!originalContentOfInterfaces.contains("" + interfaceName + ".cfg"))  //we are searching eth0.cfg or wlan0.cfg in the interfaces file
    {
        //it means no source cfg added to the interfaces file before. Add it:
        writerToInterfacesFile << "source " + configfile.fileName() + "\n";
    }
}

void NetworkHandler::ifDownIfUp(QString interfaceName){
    QProcess process;
    QString ifdownifup = "ifdown " + interfaceName + "; ifup " + interfaceName;
    process.start("bash", QStringList() << "-c" << ifdownifup);
    process.waitForFinished();
    if(!process.exitStatus() == QProcess::NormalExit){
        qDebug() << process.readAllStandardError();
    }

}
