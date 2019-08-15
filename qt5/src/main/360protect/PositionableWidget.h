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
template<typename T,typename=std::enable_if_t<std::is_base_of<QWidget,T>::value>>
class PositionableWidget : public T {
  W_OBJECT(PositionableWidget)
public:
  PositionableWidget(QWidget *parent = nullptr) : T(parent) {
  }

  QSize position() const {
    return _position;
  }

  void setPosition(QSize position) {
    _position = position;
    auto rect = this->geometry();
    this->setGeometry(QRect(_position.width(), _position.height(), rect.width(), rect.height()));

    emit positionChanged();
  }

  QSize size() const {
    return _size;
  }

  void setSize(QSize size) {
    _size = size;
    auto rect = this->geometry();
    this->setFixedSize(_size);
    this->setGeometry(QRect(rect.x(), rect.y(), _size.width(), _size.height()));

    emit sizeChanged();
  }

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


