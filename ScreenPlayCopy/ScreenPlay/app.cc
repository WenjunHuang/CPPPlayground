//
// Created by rick on 2021/10/29.
//

#include "app.h"
#include <QFontDatabase>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

namespace ScreenPlay {
App::App() : QObject(nullptr) {
  //  QGuiApplication::setWindowIcon(QIcon(":/assets/icons/app.ico"));
  QGuiApplication::setOrganizationName("ScreenPlay");
  QGuiApplication::setApplicationName("ScreenPlay");
  QGuiApplication::setApplicationVersion("0.14.0");
  QGuiApplication::setQuitOnLastWindowClosed(false);

  QFontDatabase::addApplicationFont(":/assets/fonts/Librebaskerville-Italic.ttf");


  QFontDatabase::addApplicationFont(":/assets/fonts/Roboto-Light.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/Roboto-Regular.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/Roboto-Thin.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/RobotoMono-Light.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/RobotoMono-Thin.ttf");

  QFontDatabase::addApplicationFont(":/assets/fonts/NotoSans-Thin.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/NotoSans-Regular.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/NotoSans-Medium.ttf");
  QFontDatabase::addApplicationFont(":/assets/fonts/NotoSans-Light.ttf");

  QFontDatabase::addApplicationFont(":/assets/fonts/NotoSansCJKkr-Regular.otf");

  QQuickWindow::setTextRenderType(QQuickWindow::TextRenderType::NativeTextRendering);

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
