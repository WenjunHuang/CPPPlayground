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
    Q_PROPERTY(QUrl url MEMBER _url)
  public:
    Exported(const QString& url) : _url{url} {}

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
    Q_PROPERTY(QString result MEMBER _result)
  public:
    HttpResult(const QString& result) : _result{result} {}
    ~HttpResult() {
        qDebug() << "HttpResult deconstructed";
    }

  private:
    QString _result;
};

class Http : public QObject {
    Q_OBJECT
  public:
    explicit Http(QObject* parent = nullptr);

    Q_INVOKABLE void fetch(const QJSValue& exported, QJSValue callback);

  private:
    QNetworkAccessManager* _network;
};
