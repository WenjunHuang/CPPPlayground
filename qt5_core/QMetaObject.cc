//
// Created by rick on 2022/1/4.
//
#include <QMetaObject>
#include <QDebug>
#include <QMetaClassInfo>
#include <QTimer>
#include <algorithm>

class Test : public QObject {
Q_OBJECT
public:
    explicit Test(QObject *parent = nullptr) : QObject{parent} {}

    void doStuff();

    void doStuff(QString name);

signals:

    void mySignal();

public slots:

    void mySlot();
};

void Test::doStuff() {

}

void Test::doStuff(QString name) {
  Q_UNUSED(name);

}

void Test::mySlot() {

}

void listProperties(QObject *obj) {
  for (int i = 0; i < obj->metaObject()->propertyCount(); ++i) {
    auto m = obj->metaObject()->property(i);
    qInfo() << "Property: " << m.name() << " is a " << m.typeName();
  }
}

void listMethods(QObject *obj) {
  for (int i = 0; i < obj->metaObject()->methodCount(); ++i) {
    QMetaMethod m = obj->metaObject()->method(i);
    qInfo() << "Method: " << m.methodSignature();
  }
}

void listChildren(QObject *obj) {
  qInfo() << "Object: " << obj;
  std::for_each(obj->children().cbegin(),
                obj->children().cend(), [](auto child) {
        qInfo() << child;
        listMethods(child);
        listProperties(child);
        qInfo("");
    });
}

int main() {
  QObject parent{};
  auto timer = new QTimer{&parent};

  // make some children
  for (int i = 0; i < 5; ++i) {
    auto t = new Test(&parent);
    t->setObjectName("Child: " + QString::number(i));
  }

  listChildren(&parent);

//  std::for_each(parent.children().cbegin(),
//                parent.children().cend(), [](auto &child) {
//        qInfo() << child;
//        qInfo() << child->metaObject()->className();
//        qInfo() << "Inherits: " << child->inherits("Test");
//
//        for (int m = 0; m < child->metaObject()->methodCount(); ++m) {
//          qInfo() << "Method: " << child->metaObject()->method(m).methodSignature();
//        }
//        qInfo("");
//    });

}

#include "QMetaObject.moc"