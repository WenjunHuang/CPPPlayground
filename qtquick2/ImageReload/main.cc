//
// Created by HUANG WEN JUN on 2020/2/13.
//
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickAsyncImageProvider>
#include <QtCore/QtCore>
#include <QtQuickControls2/QQuickStyle>
#include "image_response.h"

class ImageProvider : public QQuickAsyncImageProvider {
  public:
    QQuickImageResponse*
    requestImageResponse(const QString& id,
                         const QSize& requestedSize) override {
        qDebug() << "request image:" << id;
        return new MyImageResponse{id};
    }
};

int main(int argc, char* argv[]) {
    QQuickStyle::setStyle("material");
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImageProvider("imageprovider", new ImageProvider);
    engine.load("qrc:/main.qml");

    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}

#include "main.moc"