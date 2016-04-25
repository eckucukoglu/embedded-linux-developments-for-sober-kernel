#include "cpumemhandler.h"

CpuMemHandler::CpuMemHandler(QObject *parent) : QObject(parent)
{

    memtest_buffer = NULL;

    numberOfCpus = getNumberOfCpus();

    oldCpuTotals = new int[numberOfCpus +1];
    oldCpuIdles = new int[numberOfCpus +1];
    cpuTotals = new int[numberOfCpus +1];
    cpuIdles = new int[numberOfCpus+ 1];

    for(int i = 0; i <= numberOfCpus; i++){
        oldCpuIdles[i] = 0;
        oldCpuTotals[i] = 0;
        cpuIdles[i] = 0;
        cpuTotals[i] = 0;
    }
    getRamStats();
}
//CpuMemHandler::~CpuMemHandler(){
//    delete oldCpuIdles;
//    delete oldCpuTotals;
//    delete cpuIdles;
//    delete cpuTotals;
//}

int CpuMemHandler::getNumberOfCpus(){
    int processorCount = 0;
    QFile f("/proc/cpuinfo");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "error when opening /proc/cpuinfo";
        return -1;
    }
    QTextStream in(&f);
    cpuinfoContent = in.readAll();
    f.close();
    lines = cpuinfoContent.split("\n");

    foreach(QString line, lines){
        if(line.contains("processor"))
            processorCount++;
    }

    return processorCount;
}

void CpuMemHandler::getRamStats(){
    QFile f("/proc/meminfo");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "error when opening /proc/meminfo";
        return;
    }

    QTextStream in(&f);
    meminfoContent = in.readAll();
    lines = meminfoContent.split("\n");
    f.close();
    total = lines[0].split(QRegExp("\\s+"))[1].toInt();
    _free = lines[1].split(QRegExp("\\s+"))[1].toInt();
    buffer = lines[3].split(QRegExp("\\s+"))[1].toInt();
    cached = lines[4].split(QRegExp("\\s+"))[1].toInt();

    used = total - (_free + buffer + cached);
    total = total / 1024;
    used = used / 1024;
}

int CpuMemHandler::getTotalRam(){
    return total;
}

double CpuMemHandler::getRamPercentage(){
    getRamStats();
    return (double)used / total;
}

void CpuMemHandler::updateCpuValues(){
    QFile f("/proc/stat");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "error when opening /proc/stat";
        return;
    }


    QTextStream in(&f);
    procStatContent = in.readAll();

    f.close();

    //Assign last values to previous total and idles
    for(int i= 0; i < numberOfCpus; i++){
        oldCpuTotals[i] = cpuTotals[i];
        oldCpuIdles[i] = cpuIdles[i];
    }

    lines = procStatContent.split("\n");    //all lines of the stat file.

    for(int i = 0; i <= numberOfCpus; i++){
        int currentTotalCount = 0;
        //current line has idle value in 5th value (4th index).
        currentLine = lines[i].split(QRegExp("\\s+"));

        //skip the 0th index since it's the name of the cpu
        //calculate total
        for(int k = 1; k <= 10; k++){

            currentTotalCount += currentLine[k].toInt();
        }
        //get idle value
        cpuIdles[i] = currentLine[4].toInt();
        cpuTotals[i] = currentTotalCount;
    }
}

double CpuMemHandler::getCpuPercentage(int i){
    //i is the element no in the /proc/stat file.
    //i=0 is for total, from 1.....n is for respective cpus.

    double cpuPercentage = (((double)cpuTotals[i] - oldCpuTotals[i]) - (cpuIdles[i] - oldCpuIdles[i])) / (cpuTotals[i] - oldCpuTotals[i]);
    return cpuPercentage;
}

QString CpuMemHandler::readAllStatFiles(){
    QString allStatsString = "";
    QDir procDir("/proc/");
    procDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QDirIterator di(procDir);
    QString currentDir;
    int numberOfProcesses = 0;

    //This loop will traverse all the directories in /proc
    while(di.hasNext()){
        currentDir = di.next();
        numberOfProcesses++;

        //Check if there is a status file
        if(QFile::exists(currentDir + "/status"))
        {
            QFile statusFile(currentDir + "/status");

            //open the statusfile
            if (!statusFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qDebug() << "Could not open " + statusFile.fileName();
                qDebug() << statusFile.errorString();
                return "";
            }

            QTextStream in(&statusFile);
            procPidStatusContent= in.readAll();

            //qDebug() << contentOfStatusFile;
            lines = procPidStatusContent.split("\n");
            foreach(QString line, lines){
                if(line.startsWith("Name:"))
                    nameEntry = line.split("\t");
                else if(line.startsWith("Pid:"))
                    pidEntry = line.split("\t");
                else if (line.startsWith("VmRSS:"))
                    memEntry = line.split("\t");
                else if (line.startsWith("xxx"))
                    cpuEntry = line.split("\t");
            }

            statusFile.close();

            if(nameEntry.size() != 0 && memEntry.size() != 0 ){
             //   if(pidEntry == (int)getpid())
                    allStatsString += nameEntry[1].trimmed() + " "  + memEntry[1].trimmed() + " " + pidEntry[1].trimmed() + "\n";
            }

        }
    }//endwhile
    allStatsString.chop(1); //last empty line is deleted.
    //qDebug() << allStatsString;
    qDebug() << numberOfProcesses;
    return allStatsString;
}

QString CpuMemHandler::tryToKillProcess(QString pid){
    QProcess process;
    qDebug() << "kill " + pid;
    process.start("kill  " + pid);

    if(!process.waitForFinished()){
        qDebug() << "error! " << process.errorString();
        return process.errorString();
    }
    else{
        qDebug() << "done!";
        return "The process "+ pid + " have been killed succesfully.";
    }
}


bool CpuMemHandler::memtest_allocate(){
    printf("size: %d\n",memtest_size);
    if(!memtest_buffer)
        memtest_buffer = (char *)malloc(memtest_size*1024);
    else
        memtest_buffer = (char *)realloc(memtest_buffer, memtest_size*1024);

   if(!memtest_buffer)
       return false;

   memset(memtest_buffer, 0, memtest_size*1024);
   memtest_size = memtest_size + 100;
   return true;
}

int CpuMemHandler::memtest_get_size()
{
    return memtest_size;
}

bool CpuMemHandler::memtest_free_buffer()
{
    if(memtest_buffer != NULL){
        free(memtest_buffer);
        memtest_buffer = NULL;
        memtest_size = 100;
        return true;
    }
    return false;
}
