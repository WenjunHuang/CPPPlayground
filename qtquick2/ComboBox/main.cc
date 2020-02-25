//
// Created by HUANG WEN JUN on 2020/2/13.
//
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuickControls2/QQuickStyle>
#include <QLoggingCategory>
#include <QSGRendererInterface>
#include <QQuickWindow>

int main(int argc, char* argv[]) {
    QLoggingCategory::setFilterRules("qt.scenegraph.general=true");
//    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::OpenGL);

    QQuickStyle::setStyle("material");
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load("qrc:/main.qml");

    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}