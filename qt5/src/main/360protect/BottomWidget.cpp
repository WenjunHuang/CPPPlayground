//
// Created by xxzyjy on 25/11/2017.
//

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include "BottomWidget.h"
#include "ui_bottomwidget.h"

BottomWidget::BottomWidget(QWidget *parent):QWidget(parent),_ui{new Ui::BottomWidget} {
  _ui->setupUi(this);
}

void BottomWidget::paintEvent(QPaintEvent *) {
  QStyleOption option;
  option.init(this);

  QPainter painter(this);
  style()->drawPrimitive(QStyle::PE_Widget,&option, &painter,this);
}

BottomWidget::~BottomWidget() = default;
