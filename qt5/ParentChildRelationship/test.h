//
// Created by rick on 2021/2/22.
//

#pragma once
#include <QDebug>
#include <QObject>
#include <QPointer>

class Test : public QObject {
    Q_OBJECT
  public:
    Test(QObject* parent = nullptr);
    ~Test();

    QPointer<QObject> widget;

    void useWidget();
};
