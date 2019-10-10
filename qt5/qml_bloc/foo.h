//
// Created by HUANG WEN JUN on 2019/10/10.
//

#pragma once

#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>

class Foo : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
  public:
    explicit Foo(QQuickItem* parent = nullptr) : QQuickItem(parent) {}

    QString name() const { return _name; }

    void setName(const QString& name) {
        _name    = name;
        auto ctx = QQmlEngine::contextForObject(this);
        qDebug() << ctx->baseUrl();
    }

  private:
    QString _name;
};
