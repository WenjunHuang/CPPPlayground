//
// Created by rick on 2021/9/26.
//
#include <qpa/qplatformscreen.h>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);
  auto screens = QGuiApplication::screens();
  for (auto screen : screens) {
    auto platform_screen = screen->handle();
    auto default_dpi = platform_screen->logicalBaseDpi();
    auto dpi = platform_screen->logicalDpi();
    qDebug() << QString("screen %1").arg(screen->name());
    qDebug() << QString("device pixel ratio: %1")
        .arg(screen->devicePixelRatio());
    qDebug() << QString("default dpi:{%1, %2}")
                    .arg(default_dpi.first)
                    .arg(default_dpi.second);
    qDebug() << QString("current dpi:{%1, %2}").arg(dpi.first).arg(dpi.second);
    qDebug() << "---------------";
  }
}