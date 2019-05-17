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
    QNetworkRequest request(QUrl(mainURL+page));
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "some/type");
    manager->post(request,params.toString().toLocal8Bit());

    qDebug() << "Request: " << mainURL+page+"?"+params.toString().toLocal8Bit();

}

void Server::slot_FinalReply(QNetworkReply *reply){
    emit signal_reply(reply->readAll());
}
