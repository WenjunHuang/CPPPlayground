//
// Created by rick on 2020/4/5.
//
#include <QApplication>
#include <QtNetwork/QtNetwork>
#include <QDebug>

int main(int argc,char *argv[]) {
    QApplication app(argc,argv);
    QNetworkAccessManager network;
    QObject::connect(&network,&QNetworkAccessManager::finished,[](QNetworkReply* reply){
        qDebug() << "finished in QNetworkAccessManager slot";
        auto bytes = reply->readAll();
        QString content{bytes};
        qDebug() << content;
        qApp->exit(0);
    });

    QNetworkRequest request;
    request.setUrl(QUrl("https://www.baidu.com"));
    auto reply = network.get(request);
    QObject::connect(reply,&QNetworkReply::finished,reply,&QObject::deleteLater);
    QObject::connect(reply,&QObject::destroyed,[](auto obj){
        qDebug() << "QNetworkReply destroyed";

      auto bytes = (static_cast<QNetworkReply*>(obj))->readAll();
      QString content{bytes};
      qDebug() << content;
    });

    return app.exec();
}