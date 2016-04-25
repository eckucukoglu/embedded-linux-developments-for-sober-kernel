#ifndef CPUMEMHANDLER_H
#define CPUMEMHANDLER_H

#include <QObject>
#include <QFile>
#include <QtCore>

class CpuMemHandler : public QObject
{
    Q_OBJECT
public:
    explicit CpuMemHandler(QObject *parent = 0);

    Q_INVOKABLE int getNumberOfCpus();
    Q_INVOKABLE void getRamStats();
    Q_INVOKABLE double getRamPercentage();
    Q_INVOKABLE int getTotalRam();
    Q_INVOKABLE void updateCpuValues();
    Q_INVOKABLE double getCpuPercentage(int i);
    Q_INVOKABLE QString readAllStatFiles();
    Q_INVOKABLE QString tryToKillProcess(QString pid);

signals:

public slots:

private:
    int numberOfCpus;
    int* oldCpuTotals;
    int* oldCpuIdles;
    int* cpuTotals;
    int* cpuIdles;
    QString cpuinfoContent;
    QString meminfoContent;
    QString procStatContent;
    QString procPidStatusContent;
    QStringList lines, currentLine;
    QStringList memEntry, pidEntry, nameEntry;
    int total, free, buffer, cached, used;

};

#endif // CPUMEMHANDLER_H
