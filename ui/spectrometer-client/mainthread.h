#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <QThread>
#include <QtWidgets>
#include <QVector>

#include "functions.h"


struct ThreadParameters{
    int cycles;
    double time_of_cycle;
    int code_channels;
    int code_width;

    QVector<double> *data_counts;    // link to data_counts;
};

class MainThread : public QThread
{
    Q_OBJECT
public:
    ThreadParameters *parameters;

public:
    MainThread(QWidget *parent = nullptr);
    void run() override;

signals:
    void signal_dataCountDone();

private slots:
    void slot_recvParameters(ThreadParameters *p);
};

#endif // MAINTHREAD_H
