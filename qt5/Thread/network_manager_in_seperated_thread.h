//
// Created by rick on 2019/10/13.
//

#pragma once

#include <QFutureInterface>
#include <QThread>
#include <QtCore/QFuture>
#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

enum class HttpMethod {
    GET,
    POST,
    PUT,
};
class HttpWorker : public QObject {
    Q_OBJECT
  public:

    HttpWorker(QObject* parent = nullptr)
        : QObject{parent}, _network{new QNetworkAccessManager} {

        _worker = new QThread;
        _worker->setObjectName("HttpWorker");
        _network->moveToThread(_worker);
        moveToThread(_worker);

        connect(this, &HttpWorker::request, this,
                [this](HttpMethod method, QNetworkRequest request,
                       QFutureInterface<QNetworkReply*> result) {
                    QNetworkReply* reply = nullptr;

                    reply = _network->get(request);
                    QObject::connect(reply, &QNetworkReply::finished,
                                     [reply, result]() mutable {
                                         qDebug() << "finished";
                                         result.reportFinished(&reply);
                                     });
                });
        _worker->start();
    }

    QFuture<QNetworkReply*> get(QNetworkRequest networkRequest) {
        QFutureInterface<QNetworkReply*> promise(QFutureInterfaceBase::Running);

        emit request(HttpMethod::GET, networkRequest, promise);

        return promise.future();
    }

  signals:
    void request(HttpMethod method, QNetworkRequest request,
                 QFutureInterface<QNetworkReply*> futureItr);

  private:
    QThread* _worker;
    QNetworkAccessManager* _network;
};
//Q_DECLARE_METATYPE(HttpMethod)
