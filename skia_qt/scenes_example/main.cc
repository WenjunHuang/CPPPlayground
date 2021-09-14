//
// Created by rick on 2021/7/29.
//
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
//#include "QSkiaOpenGlWindow.h"
#include "Scene.h"
#include "glfw_window.h"

int main(int argc, char* argv[]) {
  //  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  //  QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
  //  QApplication app{argc, argv};
  //
  //  auto screen = QApplication::primaryScreen();
  //  auto screenSize = screen->availableSize();
  //  qDebug() << screen->devicePixelRatio();
  //  qDebug() << screenSize;
  //
  //  Scene::initializeResources();
  //  QSkiaOpenGLWindow window{1.0f};
  //  window.setScreen(screen);
  //  window.resize(static_cast<int>(screenSize.width() *
  //  0.75),static_cast<int>(screenSize.height() * 0.75)); window.show();
  //
  //  return app.exec();
  if (!glfwInit()) {
    // Initialization failed
    exit(1);
  }
  auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  int w = (int)(vidmode->width * 0.75);
  int h = (int)(vidmode->height * 0.75);
  int xpos = std::max(0, (vidmode->width - w) / 2);
  int ypos = std::max(0, (vidmode->height - h) / 2);

  GLFWWindow window;
  Scene::initializeResources();
  window.Run(xpos, ypos, w, h);
}