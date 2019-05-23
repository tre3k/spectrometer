#include "logfiles.h"

Logfiles::Logfiles(QObject *parent) : QObject(parent)
{
   logstream.setDevice(&logfile);
}

void Logfiles::writeDate(){
    if(!QDir(logpath).exists()) QDir().mkdir(logpath);
    logfile.setFileName(QDir::toNativeSeparators(logpath+"/"+logfilename));
    logfile.open(QIODevice::WriteOnly | QIODevice::Append);
    logstream << dt.currentDateTime().date().toString() << " "
              << dt.currentDateTime().time().toString() << " ";
    logfile.close();
    return;
}

void Logfiles::write(QString str){
    if(!QDir(logpath).exists()) QDir().mkdir(logpath);
    logfile.setFileName(QDir::toNativeSeparators(logpath+"/"+logfilename));
    logfile.open(QIODevice::WriteOnly | QIODevice::Append);
    logstream << str << "\n";
    logfile.close();
    return;
}

void Logfiles::setDataFileName(QString prefix){
    datafilename = prefix+"_"+dt.currentDateTime().date().toString("dd-MM-yyyy")+"_"+dt.currentDateTime().time().toString("hhmmss")+".dat";
    write("\tData file name: "+datafilename);
}

void Logfiles::saveData(QString title,QVector<double> *dat1,QVector<double> *dat2){
    if(!QDir(logpath).exists()) QDir().mkdir(logpath);
    QTextStream stream(&datafile);
    datafile.setFileName(QDir::toNativeSeparators(logpath+"/"+datafilename));
    datafile.open(QIODevice::WriteOnly);

    stream << title << "\n";
    for(int i=0;i<dat2->size();i++){
        stream << QString::number(dat1->at(i)) << "\t" << QString::number(dat2->at(i)) << "\n";
    }

    datafile.close();
}
