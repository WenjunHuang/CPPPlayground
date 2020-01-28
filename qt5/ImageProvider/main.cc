//
// Created by HUANG WEN JUN on 2020/1/20.
//
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtCore>
#include "my_image_provider.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    MyImageProvider *imageProvider = new MyImageProvider;
    engine.addImageProvider("myimageprovider",imageProvider);
    engine.load(QUrl("qrc:/main.qml"));
    app.exec();
}