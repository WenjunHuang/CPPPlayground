//
// Created by xxzyjy on 25/11/2017.
//

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include "BottomWidget.h"

struct AnimationButtonPrivate{

};

BottomWidget::BottomWidget(QWidget *parent):QWidget(parent) {
  auto layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->addStretch();

  auto barLabel = new QLabel(this);
  barLabel->setObjectName("lowBar");
  barLabel->setFixedHeight(32);
  layout->addWidget(barLabel);
}

void BottomWidget::paintEvent(QPaintEvent *) {
  QStyleOption option;
  option.init(this);

  QPainter painter(this);
  style()->drawPrimitive(QStyle::PE_Widget,&option, &painter,this);

//  painter.save();
//  painter.setPen(QColor(218,218,218));
//  painter.drawLine(QPoint(0,418),QPoint(900,418));
//
//  if (_isPaint){
//    painter.setBrush(QColor(242,244,247));
//  }
}
