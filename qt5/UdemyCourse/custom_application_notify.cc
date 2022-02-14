//
// Created by rick on 2022/1/26.
//
#include <QApplication>
#include <QDebug>
#include <QWidget>
#include <QDebug>
#include <QMetaEnum>
#include <Windows.h>

class Application : public QApplication {
public:
    Application(int &argc, char *argv[]) : QApplication{argc, argv} {}

    bool notify(QObject *object, QEvent *event) override {
      static auto kEventEnumIndex = QEvent::staticMetaObject.indexOfEnumerator("Type");

      if (event->spontaneous()) {
        auto name = QEvent::staticMetaObject.enumerator(kEventEnumIndex).valueToKey(event->type());
        qInfo() << "event notified:" << name;
      }

      return QApplication::notify(object, event);
//      return true;
    }
};

int main(int argc, char *argv[]) {
  Application app{argc, argv};
//  QWidget widget{};
//  widget.setWindowTitle("Custom App Notify");
//  widget.setStyleSheet("background-color:#282a36");
//  widget.show();

  HWND wnd = CreateWindow("MyWindow",    // classname
                          "MyWindow",    // window name
                          0,                 // style
                          0, 0, 0, 0,        // geometry
                          HWND_MESSAGE,            // parent
                          0,                 // menu handle
                          GetModuleHandle(0),     // application
                          0);                // windows creation data.

  return app.exec();
}