#include "logfiles.h"

Logfiles::Logfiles(QObject *parent) : QObject(parent)
{
   logstream.setDevice(&logfile);
}

void Logfiles::writeDate(){
    logfile.setFileName(logfilename);
    logfile.open(QIODevice::WriteOnly | QIODevice::Append);
    logstream << "\n" << dt.currentDateTime().date().toString() << " "
                      << dt.currentDateTime().time().toString() << " ";
    logfile.close();
    return;
}

void Logfiles::write(QString str){
    logfile.setFileName(logfilename);
    logfile.open(QIODevice::WriteOnly | QIODevice::Append);
    logstream << str << "\n";
    logfile.close();
    return;
}
