//
// Created by rick on 2022/1/21.
//
#include <QtWidgets/QPushButton>
#include <QApplication>
#include <QDebug>
#include <QMetaEnum>
#include <QMouseEvent>

class EventFilter : public QObject {
Q_OBJECT
public:
    EventFilter(QObject *parent = nullptr) : QObject{parent} {}

protected:
    bool eventFilter(QObject *watched, QEvent *event) override {
      static int eventEnumIndex = QEvent::staticMetaObject
        .indexOfEnumerator("Type");
      QString name = QEvent::staticMetaObject.enumerator(eventEnumIndex).valueToKey(event->type());
//      qDebug() << "get event " << name;

      switch (event->type()) {
        case QEvent::MouseButtonPress:
          return true;
        case QEvent::MouseButtonDblClick:
          return true;
      }
      return QObject::eventFilter(watched, event);
    }
};

int main(int argc, char *argv[]) {
  QApplication app{argc, argv};
  EventFilter eventFilter;
  QPushButton button{"Click Me"};
  button.installEventFilter(&eventFilter);
  QObject::connect(&button, &QPushButton::clicked, []() {
      qInfo() << "Button Clicked";
  });
  button.show();

  return QApplication::exec();

}

#include "event_filter.moc"