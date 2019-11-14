//
// Created by rick on 2019/10/13.
//

#include "network_manager_in_seperated_thread.h"
#include <QCoreApplication>
#include <QDebug>


int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    qRegisterMetaType<HttpMethod>("HttpMethod");
    qRegisterMetaType<QFutureInterface<QNetworkReply*>>("QFutureInterface<QNetworkReply*>");
    auto worker = new HttpWorker;
    auto sinaFuture =
        worker->get(QNetworkRequest(QUrl("https://news.sina.com.cn")));
    auto bilibiFuture =
        worker->get(QNetworkRequest(QUrl("https://www.bilibili.com/")));

    sinaFuture.waitForFinished();
    bilibiFuture.waitForFinished();

    auto reply = sinaFuture.result();

    QString sinaResponse(reply->readAll());
    qDebug() << sinaResponse;
    reply->deleteLater();
    //    qDebug() << "sina:" << sinaFuture.result();
    //    qDebug() << "bilibli:" << bilibiFuture.result();
}