#ifndef LOGFILES_H
#define LOGFILES_H

#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QVector>
#include <QDir>

class Logfiles : public QObject
{
    Q_OBJECT

private:
    QFile logfile;
    QFile datafile;
    QTextStream logstream;


    QDateTime dt;

public:
    QString logfilename;
    QString datafilename;
    QString logpath;
public:
    Logfiles(QObject *parent = nullptr);
    void write(QString str);
    void writeDate();
    void saveData(QString title, QVector<double> *dat1,QVector<double> *dat2);
    void setDataFileName(QString prefix);
};

#endif // LOGFILES_H
