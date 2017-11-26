#pragma once

#include <QWidget>
#include <QDebug>
#include <type_traits>
#include "wobjectdefs.h"

template<typename T>
struct is_qwidget : std::conditional<std::is_base_of<QWidget, T>::value,
  std::true_type,
  std::false_type>::type {
};

//template<typename T, typename = void>
//class PositionableWidget;
template<typename T>
class PositionableWidget : public T {
  W_OBJECT(PositionableWidget)
public:
  PositionableWidget(QWidget *parent = nullptr) : T(parent) {
    this->connect(this, SIGNAL(positionChanged()), SLOT(onPositionChanged()));
    this->connect(this, SIGNAL(sizeChanged()), SLOT(onSizeChanged()));
  }

  QSize position() const {
    return _position;
  }

  void setPosition(QSize position) {
    _position = position;
    emit positionChanged();
  }

  QSize size() const {
    return _size;
  }

  void setSize(QSize size) {
    _size = size;
    emit sizeChanged();
  }

public:

  void onPositionChanged() {
    auto rect = this->geometry();
    qDebug() << _position;
    this->setGeometry(QRect(_position.width(), _position.height(), rect.width(), rect.height()));
  }

  W_SLOT(onPositionChanged)

  void onSizeChanged() {
    auto rect = this->geometry();
    this->setFixedSize(_size);
    this->setGeometry(QRect(rect.x(), rect.y(), _size.width(), _size.height()));
  }

  W_SLOT(onSizeChanged)

public:
  void positionChanged() W_SIGNAL(positionChanged)

  void sizeChanged() W_SIGNAL(sizeChanged)

private:
  QSize _position;
  QSize _size;
public:
  W_PROPERTY(QSize, position
    READ
    position
    WRITE
    setPosition
    NOTIFY
    positionChanged)

  W_PROPERTY(QSize, size
    READ
    size
    WRITE
    setSize
    NOTIFY
    sizeChanged)
};


