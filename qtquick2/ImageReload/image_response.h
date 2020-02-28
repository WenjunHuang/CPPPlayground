//
// Created by HUANG WEN JUN on 2020/2/28.
//

#pragma once
#include <QtCore/QtCore>
#include <QQuickImageResponse>
#include <QQuickTextureFactory>

class MyImageResponse : public QQuickImageResponse {
    Q_OBJECT
  public:
    explicit MyImageResponse(const QString& imageId) : imageId_{imageId} {
        emit finished();
    }

    QQuickTextureFactory* textureFactory() const override;

  private:
    QString imageId_;
};