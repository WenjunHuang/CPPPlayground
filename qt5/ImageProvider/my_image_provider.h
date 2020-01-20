//
// Created by HUANG WEN JUN on 2020/1/20.
//

#pragma once
#include <QNetworkAccessManager>
#include <QQuickImageProvider>
#include <memory>
#include "http_worker.h"

class MyImageResponse : public QQuickImageResponse {
    Q_OBJECT
  public:
    MyImageResponse(QString const& id, QSize const& requestedSize,
                    HttpWorker* worker);
    QQuickTextureFactory* textureFactory() const override;

  private:
    QString _errors;
    QImage _image;
};

class MyImageProvider : public QQuickAsyncImageProvider {
  public:
    MyImageProvider();

    QQuickImageResponse*
    requestImageResponse(QString const& id,
                         QSize const& requestedSize) override;


  private:
    HttpWorker *_httpWorker;
};
