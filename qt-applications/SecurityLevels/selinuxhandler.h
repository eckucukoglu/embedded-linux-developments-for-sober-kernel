#ifndef SELINUXHANDLER_H
#define SELINUXHANDLER_H

#include <QObject>
#include <QtCore>
#include "selinux/selinux.h"


class SELinuxHandler : public QObject
{
    Q_OBJECT
public:
    explicit SELinuxHandler(QObject *parent = 0);

    Q_INVOKABLE QString getStatus();

    Q_INVOKABLE void setMode(int num);


signals:

public slots:
};

#endif // SELINUXHANDLER_H
