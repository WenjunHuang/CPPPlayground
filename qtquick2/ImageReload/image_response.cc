//
// Created by HUANG WEN JUN on 2020/2/28.
//

#include "image_response.h"

QQuickTextureFactory* MyImageResponse::textureFactory() const {
    return QQuickTextureFactory::textureFactoryForImage(
        QImage(QString(":/%1").arg(imageId_)));
}
