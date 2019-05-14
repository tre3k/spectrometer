#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <QThread>
#include <QtWidgets>
#include <QVector>

#include "functions.h"
#include "server.h"


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
    QNetworkAccessManager *NetworkManager;
    QNetworkCookieJar *NetworkCookie;

public:
    MainThread(QWidget *parent = nullptr);
    void run() override;

signals:
    void signal_dataCountDone();
    void signal_progressBar(int);

private slots:
    void slot_recvParameters(ThreadParameters *p);
};

#endif // MAINTHREAD_H
