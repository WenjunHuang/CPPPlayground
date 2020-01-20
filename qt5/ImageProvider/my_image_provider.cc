//
// Created by HUANG WEN JUN on 2020/1/20.
//

#include "my_image_provider.h"
#include <QNetworkReply>
#include <QPainter>

MyImageProvider::MyImageProvider() : _httpWorker{new HttpWorker} {}

QQuickImageResponse*
MyImageProvider::requestImageResponse(QString const& id,
                                      QSize const& requestedSize) {
    auto response = new MyImageResponse(id, requestedSize, _httpWorker);
    return response;
}

MyImageResponse::MyImageResponse(QString const& id, QSize const& requestedSize,
                                 HttpWorker* httpWorker) {
    QUrl url(id);
    if (!url.isValid()) {
        _errors = QString(u8"%1 不是合法的url").arg(id);
        emit finished();
    } else {
        auto future = httpWorker->get(url);
        future.wait();
        auto reply = future.get();

        connect(reply, &QNetworkReply::finished, [reply, this]() {
            if (reply->error() == QNetworkReply::NoError) {
                auto bytes = reply->readAll();
                qDebug() << "image loaded";
                _image.loadFromData(bytes);
            } else {
                _errors = reply->errorString();
            }

            emit finished();

            reply->deleteLater();
        });
    }
}

QQuickTextureFactory* MyImageResponse::textureFactory() const {
    return QQuickTextureFactory::textureFactoryForImage(_image);
}
