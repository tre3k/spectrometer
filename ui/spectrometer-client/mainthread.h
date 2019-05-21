#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <QThread>
#include <QtWidgets>
#include <QVector>

#include "functions.h"
#include "server.h"

#define REPLY_DELAY busy = true; while(busy){this->msleep(100);}

struct ThreadParameters{
    int cycles;
    double time_of_cycle;
    int code_channels;
    int code_width;

    QVector<double> *data_counts;    // link to data_counts

    QString mainURL;
    int channels;
};

class MainThread : public QThread
{
    Q_OBJECT
public:
    ThreadParameters *parameters;
private:
    bool busy = false;

public:
    MainThread(QWidget *parent = nullptr);
    void run() override;

signals:
    void signal_dataCountDone();
    void signal_progressBar(int);

    void signal_Start();
    void signal_Stop();
    void signal_ReadMem();
    void signal_Init();

private slots:
    void slot_recvParameters(ThreadParameters *p);
    void slot_reply(QByteArray);

};

#endif // MAINTHREAD_H
