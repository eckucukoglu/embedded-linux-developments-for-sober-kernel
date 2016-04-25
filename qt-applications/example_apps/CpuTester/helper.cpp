#include "helper.h"

Helper::Helper(QObject *parent) : QObject(parent)
{
}



void Helper::calculate(){
    int pid;
    if ((pid = fork()) < 0) {
        cout << "Error\n";
    }
    else if (pid == 0) {
        char * args[10];
        args[0] = "0";
        args[1] = "9999999999";
        execv("/usr/bin/matho-primes", args);
    }
    else {
        system("ls");
    //system("kill PID");
    }
}


