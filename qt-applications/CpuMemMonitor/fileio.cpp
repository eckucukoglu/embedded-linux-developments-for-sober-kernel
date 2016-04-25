#include "fileio.h"
#include <QFile>
#include <QTextStream>
#include <QtCore>

FileIO::FileIO(QObject *parent) :
    QObject(parent)
{

}

QString FileIO::read()
{
    if (mSource.isEmpty()){
        emit error("source is empty");
        return QString();
    }

    QFile file(mSource);
    QString fileContent;
    if ( file.open(QIODevice::ReadOnly) ) {
        QString line;
        QTextStream t( &file );
        do {
            line = t.readLine();
            fileContent += line + "\n";
        } while (!line.isNull());

        file.close();
    } else {
        emit error("Unable to open the file");
        return QString();
    }
    return fileContent;
}

bool FileIO::write(const QString& data)
{
    if (mSource.isEmpty())
        return false;

    QFile file(mSource);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return false;

    QTextStream out(&file);
    out << data;

    file.close();

    return true;
}

QString FileIO::readAllStatFiles(){
    QString allStatsString = "";
    QDir procDir("/proc/");
    procDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QDirIterator di(procDir);

    //This loop will traverse all the directories in /proc
    while(di.hasNext()){
        QString currentDir = di.next();

        //Check if there is a stat file
        if(QFile::exists(currentDir + "/stat"))
        {
            QFile statusFile(currentDir + "/status");
            QFile statFile(currentDir + "/stat");

            //open the statusfile
            if (!statusFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qDebug() << "Could not open " + statusFile.fileName();
                qDebug() << statusFile.errorString();
                return "";
            }

            //open the statfile
            if (!statFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qDebug() << "Could not open " + statFile.fileName();
                qDebug() << statFile.errorString();
                return "";
            }


            QTextStream in(&statusFile);
            QTextStream in2(&statFile);
            QString contentOfStatusFile = in.readAll();
            QString contentOfStatFile = in2.readAll();
            QStringList entriesOfStat = contentOfStatFile.split(" ");


            //qDebug() << contentOfStatusFile;
            QStringList entries = contentOfStatusFile.split("\n");
            QStringList nameEntry, pidEntry, memEntry;

            foreach(QString line, entries){
                if(line.startsWith("Name:"))
                     nameEntry = line.split("\t");
                else if(line.startsWith("Pid:"))
                    pidEntry = line.split("\t");
                else if (line.startsWith("VmRSS:"))
                    memEntry = line.split("\t");
            }

            statusFile.close();
            statFile.close();

            if(nameEntry.size() != 0 && pidEntry.size() != 0 && memEntry.size() != 0 )
                allStatsString += nameEntry[1].trimmed() + " " + pidEntry[1].trimmed() + " " + memEntry[1].trimmed() + "\n";

        }
    }//endwhile
    allStatsString.chop(1); //last empty line is deleted.
    //qDebug() << allStatsString;

    return allStatsString;
}

QString FileIO::tryToKillProcess(QString pid){
    QProcess process;
    qDebug() << "kill " + pid;
    process.start("kill  " + pid);

    if(!process.waitForFinished())
        qDebug() << "error! " << process.errorString();
    else
        qDebug() << "done!";
}
