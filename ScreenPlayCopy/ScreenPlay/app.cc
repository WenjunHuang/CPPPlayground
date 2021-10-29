//
// Created by rick on 2021/10/29.
//

#include "app.h"
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>

namespace ScreenPlay {
App::App() : QObject(nullptr) {
  //  QGuiApplication::setWindowIcon(QIcon(":/assets/icons/app.ico"));
  QGuiApplication::setOrganizationName("ScreenPlay");
  QGuiApplication::setApplicationName("ScreenPlay");
  QGuiApplication::setApplicationVersion("0.14.0");

  qRegisterMetaType<QQmlApplicationEngine*>();
}
void App::init() {
  mainWindowEngine_ = std::make_unique<QQmlApplicationEngine>();

  qmlRegisterSingletonInstance("ScreenPlay", 1, 0, "ScreenPlay", this);

  mainWindowEngine_->load(QUrl(QStringLiteral("qrc:/main.qml")));
}
QString App::version() const {
  return QGuiApplication::applicationVersion();
}
}  // namespace ScreenPlay
