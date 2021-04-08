//
// Created by rick on 2021/2/22.
//

#include "test.h"

Test::Test(QObject* parent) : QObject(parent) {
}
Test::~Test() {
}


void Test::useWidget() {
    if (!widget.data()) {
        qInfo() << "No Pointer!!!!";
        return;
    }

    qInfo() << "Widget = " << widget.data();

    widget.data()->setObjectName("use widget!!");
}