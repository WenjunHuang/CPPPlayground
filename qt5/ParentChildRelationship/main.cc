//
// Created by rick on 2021/2/22.
//
#include "Test.h"
#include <QCoreApplication>
#include <QDebug>
#include <QPointer>

template<class T,class U>
concept Derived = std::is_base_of<U,T>::value;

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    QObject* obj = new QObject();
    obj->setObjectName("My Object");

    QPointer<QObject> p(obj);

    Test t;
    t.widget = p;
    t.useWidget();

    if (p.data())
        qInfo() <<"p" << p.data();


    delete obj;
    t.useWidget();

    return app.exec();
}