//
// Created by HUANG WEN JUN on 2019/11/30.
//
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtCore>
#include <QQuickStyle>

int main(int argc,char * argv[]) {
    QGuiApplication app(argc,argv);
    QQmlApplicationEngine engine;
    engine.load("qrc:example.qml");

    return app.exec();
}