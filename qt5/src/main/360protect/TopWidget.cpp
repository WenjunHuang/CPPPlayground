//
// Created by xxzyjy on 25/11/2017.
//

#include <QStyleOption>
#include <QPainter>
#include "TopWidget.h"
#include "PositionableWidget.h"
#include "ThreeStateButton.h"

TopWidget::TopWidget(QWidget *parent) {
  auto btnReturn = new PositionableWidget<ThreeStateButton>(this);
  btnReturn->setObjectName("returnBtn");
}

void TopWidget::paintEvent(QPaintEvent *) {
  QStyleOption option;
  option.initFrom(this);

  QPainter painter(this);
  style()->drawPrimitive(QStyle::PE_Widget,&option,&painter,this);
}
