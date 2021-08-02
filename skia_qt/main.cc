//
// Created by rick on 2021/7/29.
//
#include "QSkiaOpenGlWindow.h"
#include "Scene.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

int main(int argc, char *argv[]) {
//  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//  QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
  QApplication app{argc, argv};
  Scene::initializeResources();

//  auto screens = QApplication::screens();
//  for(const auto& screen : screens){
//    qDebug() << screen->physicalSize();
//   qDebug() << screen->availableGeometry().width();
//  }
  auto screen = QApplication::primaryScreen();
  auto screenSize = screen->availableSize();

  QSkiaOpenGLWindow window{1.2f};
  window.setScreen(screen);
  window.resize(static_cast<int>(screenSize.width() * 0.75),static_cast<int>(screenSize.height() * 0.75));
  window.show();

  return app.exec();
}