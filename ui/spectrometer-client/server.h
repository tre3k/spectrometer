#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUrl>
#include <QtNetwork>
#include <QString>

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject *parent = nullptr);
};

#endif // SERVER_H
