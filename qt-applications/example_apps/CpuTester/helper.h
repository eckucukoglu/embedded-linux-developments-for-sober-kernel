#ifndef HELPER_H
#define HELPER_H
#include <QObject>
#include <stdio.h>
#include <stdlib.h>

class Helper: public QObject
{
    Q_OBJECT
public:
   explicit Helper(QObject *parent = 0);

   Q_INVOKABLE void calculate();


};
#endif // HELPER_H
