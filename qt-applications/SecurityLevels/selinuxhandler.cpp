#include "selinuxhandler.h"
#include <QFile>

SELinuxHandler::SELinuxHandler(QObject *parent) : QObject(parent)
{

}

QString SELinuxHandler::getStatus(){
    if(!is_selinux_enabled())
        return "DISABLED";
    else{
        int a = security_getenforce();
        if(a == 1)
            return "ENFORCING";
        else if (a == 0){
            return "PERMISSIVE";
        }
        else{
            return "UNKNOWN";
        }

    }
}

void SELinuxHandler::setMode(int num){

    if(num == 0)
    {
        //Set permissive for this session first:
        int code = security_setenforce(0);
        if(code == 0)
            qDebug() << "done!";
        else if(code == -1)
            qDebug() << "There is an error while trying to switch to permissive 'in this session.'";
    }
    else if(num == 1)
    {
        //Set enforcing for this session first:
        int code = security_setenforce(1);
        if(code == 0)
            qDebug() << "done!";
        else if(code == -1)
            qDebug() << "There is an error while trying to switch to permissive 'in this session.'";
    }


    //Now change config file so that the preference is available after reboot.

    QDir configDir("/etc/selinux");

    //If there is a config file in the directory:

    if(QFile::exists(configDir.absolutePath()+ "/config"))
    {
        QFile configFile(configDir.absolutePath()+ "/config");

        //open the config file
        if (!configFile.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            qDebug() << "Could not open " + configFile.fileName();
            qDebug() << configFile.errorString();
        }
        QTextStream in(&configFile);
        QString contentOfConfigFile;
        while(!in.atEnd()){
            QString line = in.readLine();
            contentOfConfigFile.append(line + "\n");
        }

        //Now, we have the content of the config file

        if(num == 1){   //we will make the mode enforcing
            if(contentOfConfigFile.contains("SELINUX=permissive")){     //If the searched line exists, simply change that line.
                QString lineThere = "SELINUX=permissive";
                QString desiredLine = "SELINUX=enforcing";

                contentOfConfigFile.replace(contentOfConfigFile.indexOf(lineThere), lineThere.size(), desiredLine);

            }
            else{                                                       //If the searched line does not exist, search for the opposite line.
                                                                        //If both of them are not there, add the desired line to the config.

                qDebug() << "no SELINUX=permissive line found in config file. Now searching SELINUX=enforcing line.";
                 if(contentOfConfigFile.contains("SELINUX=enforcing")){
                     //Then do nothing.
                 }
                 else{
                     //Neither of two lines are found. Add enforcing line manually to the end of the file.
                     contentOfConfigFile.append("SELINUX=enforcing\n");
                 }
            }
        }
        else if(num == 0){  //we will make the mode permissive
            if(contentOfConfigFile.contains("SELINUX=enforcing")){      //If the searched line exists, simply change that line.
                QString lineThere = "SELINUX=enforcing";
                QString desiredLine = "SELINUX=permissive";

                contentOfConfigFile.replace(contentOfConfigFile.indexOf(lineThere), lineThere.size(), desiredLine);

            }
            else{                                                       //If the searched line does not exist, search for the opposite line.
                                                                        //If both of them are not there, add the desired line to the config.

                qDebug() << "no SELINUX=enforcing line found in config file. Now searching SELINUX=permissive line..";
                if(contentOfConfigFile.contains("SELINUX=permissive")){
                    //Then do nothing.
                }
                else{
                    //Neither of two lines are found. Add enforcing line manually to the end of the file.
                    contentOfConfigFile.append("SELINUX=permissive\n");
                }
            }
        }
        else{
            qDebug() << "invalid number for setMode method.";
        }

        configFile.resize(0);
        if(configFile.write(contentOfConfigFile.toUtf8()) == -1)
            qDebug() << "error when writing into configfile";

        configFile.close();
    }
    else{
        //Config file does not exist. Create one and add the required lines
        QFile configFile(configDir.absolutePath()+ "/config");
        if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Could not open " + configFile.fileName();
            qDebug() << configFile.errorString();
        }
        QTextStream stream(&configFile);
        //Writing the selinuxtype to the config file
        stream << "SELINUXTYPE=targeted" << endl;

        //Writing the selinux mode to the config file.
        if(num == 0)
            stream << "SELINUX=permissive" << endl;
        else if(num == 1)
            stream << "SELINUX=enforcing" << endl;

    }
}
