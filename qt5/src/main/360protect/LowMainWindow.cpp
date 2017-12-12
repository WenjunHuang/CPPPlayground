//
// Created by xxzyjy on 19/11/2017.
//

#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include "LowMainWindow.h"
#include "PositionableWidget.h"
#include "wobjectimpl.h"

W_OBJECT_IMPL((PositionableWidget<T,S>), template <typename T,typename S>)

LowMainWindow::LowMainWindow(QWidget *parent) : QWidget(parent) {
  setWindowFlags(Qt::FramelessWindowHint);

  _killAndFixBtn = new PositionableWidget<AnimationButton>(this);
  _killAndFixBtn->setObjectName("killAndFixBtn");

  auto label = new PositionableWidget<QLabel>(this);
  label->setObjectName("killAndFixLabel");

  _cleanBtn = new PositionableWidget<AnimationButton>(this);
  _cleanBtn->setObjectName("cleanBtn");

  auto label1 = new PositionableWidget<QLabel>(this);
  label1->setObjectName("cleanLabel");

  _speedUpBtn = new PositionableWidget<AnimationButton>(this);
  _speedUpBtn->setObjectName("speedUpBtn");

  auto label2 = new PositionableWidget<QLabel>(this);
  label2->setObjectName("speedUpLabel");

  auto btn3 = new PositionableWidget<TwoStateButton>(this);
  btn3->setObjectName("rjgj");
  btn3->setText("软件管家");

  auto btn4 = new PositionableWidget<TwoStateButton>(this);
  btn4->setObjectName("rgfw");
  btn4->setText("人工服务");

  auto btn5 = new PositionableWidget<TwoStateButton>(this);
  btn5->setObjectName("kdcsq");
  btn5->setText("宽带测速器");

  auto btn6 = new PositionableWidget<TwoStateButton>(this);
  btn6->setObjectName("360wd");
  btn6->setText("360问答");

  auto btn7 = new PositionableWidget<ThreeStateButton>(this);
  btn7->setObjectName("moreBtn");

  auto btn8 = new PositionableWidget<QPushButton>(this);
  btn8->setObjectName("more");
  btn8->setCursor(Qt::PointingHandCursor);
  btn8->setText("更多");

  auto btn9 = new PositionableWidget<QPushButton>(this);
  btn9->setObjectName("win10Btn");
  btn9->setCursor(Qt::PointingHandCursor);
}

void LowMainWindow::paintEvent(QPaintEvent *) {
  QStyleOption option;
  option.initFrom(this);

  QPainter painter(this);
  style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
