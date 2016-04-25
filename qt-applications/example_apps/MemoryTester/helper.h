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

   Q_INVOKABLE int get_size();
   Q_INVOKABLE bool allocate();
   Q_INVOKABLE bool free_buffer();
private:
    int size = 0;
    char* buffer = NULL;
};

#endif // HELPER_H
