#ifndef LOGFILES_H
#define LOGFILES_H

#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QTextStream>

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
public:
    Logfiles(QObject *parent = nullptr);
    void write(QString str);
    void writeDate();
    void saveData();
};

#endif // LOGFILES_H
