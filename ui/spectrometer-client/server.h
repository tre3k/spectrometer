#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUrl>
#include <QtNetwork>
#include <QString>

#include <options.h>

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject *parent = nullptr);
    void setURL(QString url);
    void Request(QString page,QUrlQuery params);

    bool ready = true;
    QByteArray content;
private:
    QString mainURL;

    QNetworkAccessManager *manager;
    QNetworkCookieJar *cookie;

private slots:
    void slot_FinalReply(QNetworkReply *reply);

signals:
    void signal_reply(QByteArray);


};

#endif // SERVER_H
