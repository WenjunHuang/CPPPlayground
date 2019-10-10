//
// Created by HUANG WEN JUN on 2019/10/10.
//

#include "http.h"

Http::Http(QObject* parent) : QObject{parent} {
    _network = new QNetworkAccessManager(this);
}

void Http::fetch(const QJSValue& exported, QJSValue callback) {
    auto obj = qobject_cast<Exported*>(exported.toQObject());
    if (obj) {
        qDebug() << "OK";
    }
    QNetworkRequest request(obj->property("url").toUrl());
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
