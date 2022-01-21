//
// Created by rick on 2021/12/31.
//
#include <QDebug>
#include <QSharedPointer>

class Test : public QObject {
Q_OBJECT
};

class Consumer: public QObject{
    Q_OBJECT
public:
    QSharedPointer<Test> tester;
};

void doDeleteLater(Test *obj) {
  qInfo() << "Deleting : " << obj;
  obj->deleteLater();
}

QSharedPointer<Test> createPointer() {
  Test *t = new Test();
  t->setObjectName("Test Object");
  QSharedPointer<Test> p(t, doDeleteLater);
  return p;
}

void doStuff(QSharedPointer<Consumer> consumer) {
  qInfo() << "In function: " << consumer.data()->tester;
  consumer.clear();
}

QSharedPointer<Consumer> makeConsumer() {
  QSharedPointer<Consumer> c(new Consumer,&QObject::deleteLater);
  QSharedPointer<Test> t = createPointer();

  c.data()->tester.swap(t);
  doStuff(c);
  return c;
}

int main() {
  QSharedPointer<Consumer> consume = makeConsumer();
  qInfo() << "In main:" <<consume.data()->tester;
  consume.clear();
}

#include "QSharedPointer.moc"

