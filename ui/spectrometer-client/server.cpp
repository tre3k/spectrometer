#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager();
    cookie = new QNetworkCookieJar();

    manager->setCookieJar(cookie);

    connect(manager,SIGNAL(finished(QNetworkReply *)),
            this,SLOT(slot_FinalReply(QNetworkReply *)));
}

void Server::setURL(QString url){
    mainURL = url;
}

void Server::Request(QString page, QUrlQuery params){
    ready = false;
    QNetworkRequest request(QUrl(mainURL+page));

    manager->post(request,params.toString().toLocal8Bit());

}

void Server::slot_FinalReply(QNetworkReply *reply){
    ready = true;
    //qDebug() << reply->readAll();
    //content =
    emit signal_reply(reply->readAll());
}
