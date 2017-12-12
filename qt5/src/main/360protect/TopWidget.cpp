//
// Created by xxzyjy on 25/11/2017.
//

#include <QStyleOption>
#include <QPainter>
#include <QStyle>
#include "TopWidget.h"
#include "ui_topwidget.h"

TopWidget::TopWidget(QWidget *parent) : QWidget{parent},_ui{new Ui::TopWidget} {
//  auto btnReturn = new PositionableWidget<ThreeStateButton>(this);
//  btnReturn->setObjectName("returnBtn");
  _ui->setupUi(this);
//  connect(btnReturn, &QPushButton::clicked, this, &TopWidget::onReturn);
}

void TopWidget::paintEvent(QPaintEvent *) {
  QStyleOption option;
  option.initFrom(this);

  QPainter painter(this);
  style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}

TopWidget::~TopWidget() =default;
