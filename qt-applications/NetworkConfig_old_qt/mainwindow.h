#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QDesktopWidget>
#include <QDebug>
#include <QtNetwork>
#include <QMessageBox>
#include <QProgressDialog>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_NetwConfComboBox_currentIndexChanged(int index);

    void on_cancelButton_clicked();

    void on_applyButton_clicked();

    void on_connTypeComboBox_currentIndexChanged(int index);

    void on_ipAddressEdit_editingFinished();

    void on_ipAddressEdit_2_editingFinished();

    void on_ipAddressEdit_3_editingFinished();

    void on_ipAddressEdit_4_editingFinished();

    void on_dnsEdit_editingFinished();

    void on_dnsEdit_2_editingFinished();

    void on_dnsEdit_3_editingFinished();

    void on_dnsEdit_4_editingFinished();

    void on_netmaskEdit_editingFinished();

    void on_netmaskEdit_2_editingFinished();

    void on_netmaskEdit_3_editingFinished();

    void on_netmaskEdit_4_editingFinished();

    void on_gatewayEdit_editingFinished();

    void on_gatewayEdit_2_editingFinished();

    void on_gatewayEdit_3_editingFinished();

    void on_gatewayEdit_4_editingFinished();

    void on_ifdownifupFinished(int, QProcess::ExitStatus);

    void on_checkConnectionButton_clicked();

    void on_timeout();

private:
    Ui::MainWindow *ui;
    QProgressDialog * commandProgressDialog;

    //The values that will be listed which are taken from system
    QString initialIP;
    QString initialNetmask;
    QString initialGateway;
    QString initialMacAddress;
    QString wirelessMacAddress;
    QString defaultConnection;

    //Values kept as a list
    QStringList ipEntryList;
    QStringList gatewayEntryList;
    QStringList netmaskEntryList;
    QStringList dnsEntryList;

    //Candidate values to be applied
    QString candidateIP;
    QString candidateDNS;
    QString candidateNetmask;
    QString candidateGateway;

    QProcess *process;
    QNetworkInterface *interface;
    QList<QNetworkInterface> list;

    QString getGatewayCommand;
    QString ifdownifup;
    QString interfacesdotDpath;
    QString resolvconfHeadpath;
    QString ifstatePath;

    QList<QLineEdit*> ipEditList;
    QList<QLineEdit*> dnsEditList;
    QList<QLineEdit*> netmaskEditList;
    QList<QLineEdit*> gatewayEditList;

    QList<QLineEdit*> lineEditList;
    void centerWindow(QWidget* widget);
    void initializeValues();
    void fillInTheLineEdits();
    void applyNetworkConfiguration();
    void applyAutoNetworkConfiguration();
    QString prepareCfg();
    QString prepareAutoCfg();
};

#endif // MAINWINDOW_H
