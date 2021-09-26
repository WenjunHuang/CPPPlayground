//
// Created by rick on 2021/9/26.
//
#include <qpa/qplatformscreen.h>
#include <QApplication>
#include <QDebug>
#include <QFormLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QScreen>
#include <QSpinBox>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  qDebug() << " Qt::AA_EnableHighDpiScaling set?"
           << app.testAttribute(Qt::AA_EnableHighDpiScaling);
  qDebug() << " Qt::AA_DisableHighDpiScaling set?"
           << app.testAttribute(Qt::AA_DisableHighDpiScaling);
  app.setAttribute(Qt::AA_EnableHighDpiScaling);
  // try to disable auto system dpi changed scaling, but not work :(

  auto screens = QGuiApplication::screens();
  for (auto screen : screens) {
    auto platform_screen = screen->handle();
    auto default_dpi = platform_screen->logicalBaseDpi();
    auto dpi = platform_screen->logicalDpi();
    auto screen_name = screen->name();
    qDebug() << QString("screen %1").arg(screen_name);
    qDebug()
        << QString("device pixel ratio: %1").arg(screen->devicePixelRatio());
    qDebug() << QString("default dpi:{%1, %2}")
                    .arg(default_dpi.first)
                    .arg(default_dpi.second);
    qDebug() << QString("current dpi:{%1, %2}").arg(dpi.first).arg(dpi.second);
    qDebug() << "---------------";
    QObject::connect(
        screen, &QScreen::logicalDotsPerInchChanged, [=](auto dpi) {
          qDebug() << QString("logical dpi changed for screen: %1 to %2")
                          .arg(screen_name)
                          .arg(dpi);
        });
  }
  QWidget window;
  //  window.connect(&window,QWindow::)
  window.setWindowTitle("Hello");
  window.setGeometry(0, 0, 300, 300);
  QFont font_with_point_size;
  font_with_point_size.setPointSize(16);

  QFont font_with_pixel_size;
  font_with_pixel_size.setPixelSize(16);

  //  window.setFixedWidth(300);
  QFormLayout* formLayout = new QFormLayout;
  auto name = new QLineEdit();
  formLayout->addRow(QObject::tr("&Name:"), name);
  formLayout->labelForField(name)->setFont(font_with_point_size);
  auto email = new QLineEdit();
  formLayout->addRow(QObject::tr("&Email:"), email);
  formLayout->labelForField(email)->setFont(font_with_pixel_size);
  auto age = new QSpinBox();
  formLayout->addRow(QObject::tr("&Age:"), age);

  window.setLayout(formLayout);
  window.show();
  return app.exec();
}