//
// Created by rick on 2021/12/31.
//
#include <QDebug>
#include <QPointer>

class test : public QObject {
Q_OBJECT
public:
    explicit test(QObject *parent = nullptr) : QObject{parent} {}

    QPointer<QObject> widget_;

    void useWidget() {
      if (!widget_.data()) {
        qInfo() << "No Pointer!!!";
        return;
      }
      qInfo() << "Widget = " << widget_.data();

      widget_.data()->setObjectName("used Widget!");
    }
};

int main() {
  QObject *obj = new QObject();
  obj->setObjectName("My Object");
  QPointer<QObject> p(obj);

  test t;
  t.widget_ = p;
  t.useWidget();

  if (p.data()) {
    qInfo() << p.data();
  }

  delete obj;
  t.useWidget();

}

#include "QPointer.moc"

