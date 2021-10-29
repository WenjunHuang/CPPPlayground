//
// Created by rick on 2021/10/29.
//
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuickControls2/QQuickStyle>
#include <QtWebEngine/QtWebEngine>
#include "app.h"

int main(int argc, char* argv[]) {
  Q_INIT_RESOURCE(ScreenPlayQML);
//  QQuickStyle::setStyle("material");

  QtWebEngine::initialize();
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

  QGuiApplication qtGuiApp(argc, argv);
  ScreenPlay::App app;
  app.init();

  return qtGuiApp.exec();
}