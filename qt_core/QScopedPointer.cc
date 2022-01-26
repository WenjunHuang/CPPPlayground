//
// Created by HUANG WEN JUN on 2021/12/31.
//

#include <QScopedPointer>
#include <QDebug>
#include <string>
#include <QCoreApplication>

class Tester : public QObject {
Q_OBJECT
public:
    explicit Tester(QObject *parent = nullptr) : QObject(parent) {
        qInfo() << "Constructed: " << this;
    }

    ~Tester() {
        qInfo() << "Deconstructed: " << this;
    }
};

class Consumer : public QObject {
Q_OBJECT
public:
    explicit Consumer(QObject *parent = nullptr) : QObject{parent} {
        qInfo() << "Constructed: " << this;
    }

    ~Consumer() {
        qInfo() << "Deconstructed: " << this;
    }

    QSharedPointer<Tester> tester;
};

void doDeleteLater(Tester *obj) {
    qInfo() << "Deleting: " << obj;
    obj->deleteLater();
}

QSharedPointer<Tester> createPointer() {
    Tester *t = new Tester{};
    t->setObjectName("Test Object");
    return {t, doDeleteLater};
}

void doStuff(QSharedPointer<Consumer> ptr) {
    qInfo() << "In function: " << ptr.data()->tester;

    ptr.clear();
}

QSharedPointer<Consumer> makeConsumer() {
    QSharedPointer<Consumer> obj{new Consumer{}, &QObject::deleteLater};
    QSharedPointer<Tester> ptr = createPointer();

    obj.data()->tester.swap(ptr);
    doStuff(obj);
    return obj;
}

int main(int argc,char *argv[]) {
    QCoreApplication app{argc,argv};
    QSharedPointer<Consumer> consume = makeConsumer();
    qInfo() << "In main: " << consume.data()->tester;

    consume.clear();
    return app.exec();
}

#include "QScopedPointer.moc"

