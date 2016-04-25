#include "mainwindow.h"
#include "ui_mainwindow.h"


//***************************************************************
// Necessary Permissions: +rwx for:
//
//          /etc/network/interfaces             (write permission)
//          /etc/network/interfaces.d/  -r      (for config files)
//          /run/network/                       (for ifstate)
//          /etc/resolvconf/resolv.conf.d/head  (write permission)
//          /sys/class/net/$c0/carrier  $c = connectionType (read permission)
//
//***************************************************************

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Network Configuration Manager");

    ui->advancedFrame->setHidden(false);
    ui->NetwConfComboBox->setCurrentIndex(0);   //Set Network Configuration Auto as default
    this->setFixedSize(size());

    //Preparing validators for the line edits.
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipPartRegex (ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipPartRegex, this);

    //Adding all LineEdits to their respective lists and to lineEditList.
    lineEditList.push_back(ui->ipAddressEdit);
    lineEditList.push_back(ui->ipAddressEdit_2);
    lineEditList.push_back(ui->ipAddressEdit_3);
    lineEditList.push_back(ui->ipAddressEdit_4);

    ipEditList.push_back(ui->ipAddressEdit);
    ipEditList.push_back(ui->ipAddressEdit_2);
    ipEditList.push_back(ui->ipAddressEdit_3);
    ipEditList.push_back(ui->ipAddressEdit_4);

    lineEditList.push_back(ui->dnsEdit);
    lineEditList.push_back(ui->dnsEdit_2);
    lineEditList.push_back(ui->dnsEdit_3);
    lineEditList.push_back(ui->dnsEdit_4);

    dnsEditList.push_back(ui->dnsEdit);
    dnsEditList.push_back(ui->dnsEdit_2);
    dnsEditList.push_back(ui->dnsEdit_3);
    dnsEditList.push_back(ui->dnsEdit_4);

    lineEditList.push_back(ui->netmaskEdit);
    lineEditList.push_back(ui->netmaskEdit_2);
    lineEditList.push_back(ui->netmaskEdit_3);
    lineEditList.push_back(ui->netmaskEdit_4);

    netmaskEditList.push_back(ui->netmaskEdit);
    netmaskEditList.push_back(ui->netmaskEdit_2);
    netmaskEditList.push_back(ui->netmaskEdit_3);
    netmaskEditList.push_back(ui->netmaskEdit_4);

    lineEditList.push_back(ui->gatewayEdit);
    lineEditList.push_back(ui->gatewayEdit_2);
    lineEditList.push_back(ui->gatewayEdit_3);
    lineEditList.push_back(ui->gatewayEdit_4);

    gatewayEditList.push_back(ui->gatewayEdit);
    gatewayEditList.push_back(ui->gatewayEdit_2);
    gatewayEditList.push_back(ui->gatewayEdit_3);
    gatewayEditList.push_back(ui->gatewayEdit_4);


    //Add validator for all text fields
    foreach(QLineEdit* e, lineEditList){
        e->setValidator(ipValidator);
    }

    //Set window position at the start:
    centerWindow(this);

    //Initializing the text edits and variables
    defaultConnection = "wlan0";    //Default Connection type is Wireless.
    interface = new QNetworkInterface();    //Creating a new QNetworkInterface object to use in filling the values
    list = interface->allInterfaces();
    interfacesdotDpath = "/etc/network/interfaces.d/";
    resolvconfHeadpath = "/etc/resolvconf/resolv.conf.d/";
    ifstatePath = "/run/network/ifstate";
    process = new QProcess(this);
    getGatewayCommand = "netstat -rn | grep 0.0.0.0 | awk '{print $2}' | grep -v '0.0.0.0'";

    ui->tickLabel->setHidden(true);
    ui->crossLabel->setHidden(true);


    initializeValues();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_NetwConfComboBox_currentIndexChanged(int index)
{
    if(index == 1)
    {

        foreach(QLineEdit* e, lineEditList){
            e->setEnabled(true);
        }

    }
    else{
        foreach(QLineEdit* e, lineEditList){
            e->setEnabled(false);
        }
    }

}

void MainWindow::on_cancelButton_clicked()
{
    this->close();
}

void MainWindow::centerWindow(QWidget* widget){
    QDesktopWidget *dw = QApplication::desktop();
    QSize size = widget->size();

    int myWidth = size.width();
    int myHeight = size.height();

    int dtWidth = dw->width();
    int dtHeight = dw->height();

    int centerW = (dtWidth - myWidth)/2;
    int centerH = (dtHeight - myHeight)/2;

    widget->move(centerW, centerH);

}

void MainWindow::initializeValues(){

    //Getting the MAC Address values
    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).name().startsWith("eth0"))
            initialMacAddress = list.at(i).hardwareAddress();
        if(list.at(i).name().startsWith("wlan0"))
            wirelessMacAddress = list.at(i).hardwareAddress();
    }

    // Getting the IP, Netmask, Gateway values
    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).name().startsWith(defaultConnection)) //wlan0 or eth0
        {

            QNetworkInterface iface = QNetworkInterface::interfaceFromName(list.at(i).name());

            if(iface.flags().testFlag(QNetworkInterface::IsRunning)){
                QList<QNetworkAddressEntry> entries = iface.addressEntries();

                if (!entries.isEmpty())
                {
                    QNetworkAddressEntry entry = entries.first();
                    initialIP = entry.ip().toString();
                    initialNetmask = entry.netmask().toString();

                }
                else{
                    initialIP = "0.0.0.0";
                    initialNetmask = "255.255.255.0";
                    initialGateway = "0.0.0.0";
                }
            }
            else{
                initialIP = "0.0.0.0";
                initialNetmask = "255.255.255.0";
                initialGateway = "0.0.0.0";
            }

        }
    }


    process->start("bash", QStringList() << "-c" << getGatewayCommand);
    process->waitForFinished();
    QString gatewayPerhaps = process->readAllStandardOutput();
    if(gatewayPerhaps.isEmpty())
        initialGateway = "0.0.0.0";
    else{
        gatewayPerhaps.chop(1);
        QHostAddress gate(gatewayPerhaps);
        QHostAddress ipadd(initialIP);
        QHostAddress netm(initialNetmask);

        if(gate.isInSubnet(QHostAddress::parseSubnet(initialIP + "/" + initialNetmask)))
        {
            initialGateway = gatewayPerhaps;
        }

        else{
            initialGateway = "0.0.0.0";
        }
    }

    //Parsing the ip, dns, netmask and gateway to fill in the separate line edits:
    ipEntryList = initialIP.split(".");
    gatewayEntryList = initialGateway.split(".");
    netmaskEntryList = initialNetmask.split(".");

    //initially, fill dns entry list with 0.0.0.0
    dnsEntryList << "0" << "0" << "0" << "0";

    fillInTheLineEdits();

}

void MainWindow::on_connTypeComboBox_currentIndexChanged(int index)
{
    foreach(QLineEdit* e, lineEditList){
        e->setText("");
    }
    if(index == 1){

        defaultConnection = "eth0";
    }

    else if(index == 0){
        defaultConnection = "wlan0";
    }


    initializeValues();

}

void MainWindow::fillInTheLineEdits(){
    ui->macAddressEdit->setText(initialMacAddress);
    ui->wiMacAddressEdit->setText(wirelessMacAddress);

    for(int i = 0; i < ipEntryList.size(); i++){
        ipEditList.at(i)->setText(ipEntryList.at(i));
        netmaskEditList.at(i)->setText(netmaskEntryList.at(i));
        gatewayEditList.at(i)->setText(gatewayEntryList.at(i));
    }
}

void MainWindow::on_ipAddressEdit_editingFinished()
{
    ipEntryList[0] = ui->ipAddressEdit->text();
}

void MainWindow::on_ipAddressEdit_2_editingFinished()
{
    ipEntryList[1] = ui->ipAddressEdit_2->text();
}

void MainWindow::on_ipAddressEdit_3_editingFinished()
{
    ipEntryList[2] = ui->ipAddressEdit_3->text();
}

void MainWindow::on_ipAddressEdit_4_editingFinished()
{
    ipEntryList[3] = ui->ipAddressEdit_4->text();
}

void MainWindow::on_dnsEdit_editingFinished()
{
    dnsEntryList[0] = ui->dnsEdit->text();
}

void MainWindow::on_dnsEdit_2_editingFinished()
{
    dnsEntryList[1] = ui->dnsEdit_2->text();
}

void MainWindow::on_dnsEdit_3_editingFinished()
{
    dnsEntryList[2] = ui->dnsEdit_3->text();
}

void MainWindow::on_dnsEdit_4_editingFinished()
{
    dnsEntryList[3] = ui->dnsEdit_4->text();
}

void MainWindow::on_netmaskEdit_editingFinished()
{
    netmaskEntryList[0] = ui->netmaskEdit->text();
}

void MainWindow::on_netmaskEdit_2_editingFinished()
{
    netmaskEntryList[1] = ui->netmaskEdit_2->text();
}

void MainWindow::on_netmaskEdit_3_editingFinished()
{
    netmaskEntryList[2] = ui->netmaskEdit_3->text();
}

void MainWindow::on_netmaskEdit_4_editingFinished()
{
    netmaskEntryList[3] = ui->netmaskEdit_4->text();
}

void MainWindow::on_gatewayEdit_editingFinished()
{
    gatewayEntryList[0] = ui->gatewayEdit->text();
}

void MainWindow::on_gatewayEdit_2_editingFinished()
{
    gatewayEntryList[1] = ui->gatewayEdit_2->text();
}

void MainWindow::on_gatewayEdit_3_editingFinished()
{
    gatewayEntryList[2] = ui->gatewayEdit_3->text();
}

void MainWindow::on_gatewayEdit_4_editingFinished()
{
    gatewayEntryList[3] = ui->gatewayEdit_4->text();
}


void MainWindow::on_applyButton_clicked()
{
    qDebug() << "Apply clicked with default connection type: " + defaultConnection;
    ifdownifup = "gksudo ifdown " + defaultConnection + "; gksudo ifup " + defaultConnection;
    //Preparing the previously parsed ip, dns, netmask, etc.
    candidateIP = ipEntryList[0] +"."+ ipEntryList[1] +"."+ ipEntryList[2] +"."+ ipEntryList[3];
    candidateDNS = dnsEntryList[0] + "." + dnsEntryList[1] + "." + dnsEntryList[2] + "." + dnsEntryList[3];
    candidateNetmask = netmaskEntryList[0] + "." + netmaskEntryList[1] + "." +netmaskEntryList[2] + "." +netmaskEntryList[3];
    candidateGateway = gatewayEntryList[0] + "." + gatewayEntryList[1] + "." + gatewayEntryList[2] + "." + gatewayEntryList[3];

    //Creating Warning Message
    QMessageBox warningBox;
    warningBox.setIcon(QMessageBox::Warning);
    QString warningBoxInfoText;

    if(ui->NetwConfComboBox->currentIndex() == 1){  //MANUAL
        warningBox.setText("This configuration might corrupt your network settings:");
        warningBoxInfoText = "Are you sure you want to apply these changes? \n\n";
        if(candidateIP != initialIP){
            if(candidateIP != "0.0.0.0")
                warningBoxInfoText += "IP Addr: " + candidateIP + "\n";
            warningBoxInfoText += "Netmask: " + candidateNetmask + "\n";
            if(candidateGateway != "0.0.0.0")
                warningBoxInfoText += "Gateway: " + candidateGateway + "\n";
        }


        if(candidateDNS != "0.0.0.0")
            warningBoxInfoText += "DNS: " + candidateDNS + "\n";
    }
    else if(ui->NetwConfComboBox->currentIndex() == 0){ //AUTO
        warningBox.setText("Network configuration will be done by your DHCP.");
        warningBoxInfoText = "Are you sure you want to continue?";
    }

    warningBox.setInformativeText(warningBoxInfoText);
    warningBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    warningBox.setDefaultButton(QMessageBox::No);
    warningBox.setWindowModality(Qt::NonModal);
    int choice = warningBox.exec();

    switch(choice){
    case QMessageBox::Yes:
        applyNetworkConfiguration();
        break;
    case QMessageBox::No:
        break;
    default:
        //shall never be reached
        break;
    }
}

//The content of the .cfg file that will be added to /etc/network/interfaces.d/ directory
QString MainWindow::prepareCfg(){
    QString contentOfTheConfig= "auto " + defaultConnection + "\n";
    if(candidateIP != initialIP){
        if(candidateIP == "0.0.0.0")
            return "";

        contentOfTheConfig += "iface " + defaultConnection + " inet static\n";
        if(candidateIP != "0.0.0.0")
            contentOfTheConfig += "\taddress " + candidateIP + "\n";
        if(candidateGateway != "0.0.0.0")
            contentOfTheConfig += "\tgateway " + candidateGateway + "\n";
        contentOfTheConfig += "\tnetmask " + candidateNetmask + "\n";
    }



    return contentOfTheConfig;
}

QString MainWindow::prepareAutoCfg(){
    QString contentOfTheConfig= "auto " + defaultConnection + "\n";
    contentOfTheConfig += "iface " + defaultConnection + " inet dhcp";

    return contentOfTheConfig;
}

void MainWindow::applyNetworkConfiguration(){
    if(ui->NetwConfComboBox->currentIndex() == 1){
        //Open the etc/network/interfaces file and get the original content of the file:
        QFile interfaces("/etc/network/interfaces");
        if (!interfaces.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "Could not open " + interfaces.fileName();
            qDebug() << interfaces.errorString();
            return;
        }

        QString originalContentOfInterfaces = "";
        QTextStream in(&interfaces);
        originalContentOfInterfaces = in.readAll();
        interfaces.close();

        //***********************************************************
        //Creating the config file:
        //***********************************************************
        QDir configs = QDir::root();
        configs.mkpath(interfacesdotDpath);
        configs.cd(interfacesdotDpath);

        //Now directory is in interfaces.d/
        QFile configfile(configs.filePath(defaultConnection + ".cfg"));

        if(!configfile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)){
            qDebug() << "Cannot create the file " + configfile.fileName();
            qDebug() << configfile.errorString();
        }

        //Preparing the cfg file to be added:
        QString configContents;
        if(ui->NetwConfComboBox->currentIndex() == 0)
            configContents = prepareAutoCfg();
        else if(ui->NetwConfComboBox->currentIndex() == 1)
            configContents = prepareCfg();

        //Adding the contents to the config file
        QTextStream configFileIn(&configfile);
        configFileIn << configContents;
        configfile.close();

        //***********************************************************
        //add the necessary connection config file's source
        //to interfaces file according to the user's inputs:
        //***********************************************************
        QTextStream writerToInterfacesFile(&interfaces);
        if (!interfaces.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)){
            qDebug() << "Could not open " + interfaces.fileName();
            qDebug() << interfaces.errorString();
            return;
        }

        //append the source or do nothing according to our inspection:
        if(!originalContentOfInterfaces.contains("" + defaultConnection + ".cfg"))  //we are searching eth0.cfg or wlan0.cfg in
            //the interfaces file.
        {
            //it means no source cfg added to the interfaces file before. Add it:
            writerToInterfacesFile << "source " + configfile.fileName() + "\n";
        }
    }
    else if(ui->NetwConfComboBox->currentIndex() == 0){
        //***********************************************************
        //Delete the source line from interfaces
        //***********************************************************
        QFile interfaces("/etc/network/interfaces");
        if (!interfaces.open(QIODevice::ReadWrite | QIODevice::Text)){
            qDebug() << "Could not open " + interfaces.fileName();
            qDebug() << interfaces.errorString();
            return;
        }

        QString contentsWithoutSourceLine;
        QTextStream interfacesStream(&interfaces);
        while(!interfacesStream.atEnd()){
            QString line = interfacesStream.readLine();
            if(!line.contains("" + defaultConnection + ".cfg"))
                contentsWithoutSourceLine.append(line + "\n");
        }
        interfaces.resize(0);
        interfacesStream << contentsWithoutSourceLine;
        interfaces.close();

        //***********************************************************
        //Delete the config file.
        //***********************************************************
        QDir configs = QDir::root();
        configs.mkpath(interfacesdotDpath);
        configs.cd(interfacesdotDpath);

        //Now directory is in interfaces.d/
        QFile configfile(configs.filePath(defaultConnection + ".cfg"));
        configfile.remove();
    }


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
        qDebug() << "Cannot create the file " + resolvConfHead.fileName();
        qDebug() << resolvConfHead.errorString();
    }

    //Preparing the dns string and adding it to the head file
    QString resolvConfHeadContents;
    if(candidateDNS != "0.0.0.0" && ui->NetwConfComboBox->currentIndex() == 1){
        resolvConfHeadContents += "nameserver " + candidateDNS + "\n";
    }
    else {
        resolvConfHeadContents = "";    //send empty string if not specified. No preference
    }

    //Add the contents to the config file
    QTextStream resolvConfHeadFileIn(&resolvConfHead);
    resolvConfHeadFileIn << resolvConfHeadContents;
    resolvConfHead.close();


    //***********************************************************
    //Add the connectionType=connectionType line into the ifstate.
    //***********************************************************


    QFile ifstateFile(ifstatePath);

    //Give necessary permissions:
    process->execute("gksudo", QStringList()<<  "chmod -R +774 /run/network/");

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
    if(!ifstateContents.contains("" + defaultConnection + "=" + defaultConnection))
        ifstateStream << defaultConnection + "=" + defaultConnection;

    ifstateFile.close();

    //***********************************************************
    //Reset the resolv.conf content
    //***********************************************************
    QString rccommand = "resolvconf -u";
    process->execute("gksudo", QStringList() << rccommand);

    //***********************************************************
    //Now update the network status by using QProcess (ifdown ifup)
    //***********************************************************
    commandProgressDialog = new QProgressDialog(this, Qt::Dialog);

    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(on_ifdownifupFinished(int, QProcess::ExitStatus)));

    commandProgressDialog->setWindowModality(Qt::WindowModal);
    commandProgressDialog->setMaximum(0);
    commandProgressDialog->setMinimum(0);
    commandProgressDialog->setCancelButton(0);
    commandProgressDialog->setDisabled(true);

    process->start("bash", QStringList() << "-c" << ifdownifup);
    commandProgressDialog->exec();
    ui->statusBar->showMessage("The Command Finished Running.", 2000);
    if(process->waitForReadyRead(-1)){
        qDebug() << "ifdown ifup command error: " + process->readAllStandardError();
        qDebug() << "ifdown ifup command output: " + process->readAllStandardOutput();
    }

}

void MainWindow::on_ifdownifupFinished(int exitCode, QProcess::ExitStatus exitStatus){
    if(exitStatus == QProcess::NormalExit){
        qDebug() << "program exitted normally";
        commandProgressDialog->setMaximum(100);
        commandProgressDialog->setMinimum(0);
        commandProgressDialog->setValue(99);
        commandProgressDialog->close();
    }
}

void MainWindow::on_checkConnectionButton_clicked()
{
    QFile carrierFile("/sys/class/net/" + defaultConnection + "/carrier");

    if(!carrierFile.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << " carrierfile file couldnt open";

    QTextStream carrierIn(&carrierFile);
    int connectedStatus = carrierIn.readAll().toInt();

    QTimer *timer = new QTimer(this);
    timer->setInterval(1500);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    timer->start();

    qDebug() << "connected status: " << connectedStatus;
    if(connectedStatus == 1){
        ui->tickLabel->setVisible(true);
    }
    else if (connectedStatus == 0)
    {
        ui->crossLabel->setVisible(true);
    }
    else{
        qDebug() << "file is empty. device is turned off";
    }


}

void MainWindow::on_timeout(){
    ui->tickLabel->setVisible(false);
    ui->crossLabel->setVisible(false);
}
