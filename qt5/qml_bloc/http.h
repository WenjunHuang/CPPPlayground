//
// Created by HUANG WEN JUN on 2019/10/10.
//

#pragma once

#include <QDebug>
#include <QJSValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <utility>

class Exported : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl)
  public:
    Exported(const QString& url) : _url{url} {}

    QString url() const { return _url.url(); }

    void setUrl(const QString& url) { _url = url; }

  private:
    QUrl _url;
};

class ExportedFactory : public QObject {
    Q_OBJECT
  public:
    Q_INVOKABLE QJSValue create(const QString& url) {
        auto rtn      = new Exported(url);
        auto jsEngine = qjsEngine(this);
        return jsEngine->newQObject(rtn);
    }
};
class HttpResult : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString result READ result)
  public:
    HttpResult(const QString& result) : _result{result} {}
    ~HttpResult() {
        qDebug() << "HttpResult deconstructed";
    }

    QString result() { return _result; }

  private:
    QString _result;
};
class Http : public QObject {
    Q_OBJECT
  public:
    explicit Http(QObject* parent = nullptr) : QObject{parent} {
        _network = new QNetworkAccessManager(this);
    }

    Q_INVOKABLE void fetch(const QJSValue& exported, QJSValue callback) {
        auto obj = qobject_cast<Exported*>(exported.toQObject());
        if (obj) {
            qDebug() << "OK";
        }
        QNetworkRequest request(QUrl(obj->url()));
        auto reply = _network->get(request);
        connect(reply, &QNetworkReply::finished,
                [callback = std::move(callback), reply, this]() mutable {
                    auto engine = qjsEngine(this);
                    QString result(reply->readAll());
                    reply->deleteLater();

                    auto resultObject = new HttpResult(result);

                    //                  auto resultObject = engine->newObject();
                    //                  resultObject.setProperty("result",
                    //                  QJSValue(result));
                    callback.call({engine->newQObject(resultObject)});
                });
    }

  private:
    QNetworkAccessManager* _network;
};
