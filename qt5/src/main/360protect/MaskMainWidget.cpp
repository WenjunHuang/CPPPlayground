//
// Created by xxzyjy on 20/11/2017.
//

#include "MaskMainWidget.h"

MaskMainWidget::MaskMainWidget(QWidget *parent):QWidget(parent) {
  setWindowFlags(Qt::FramelessWindowHint);
  setFixedSize(QSize(900,600));

  _upmain = new UpMainWindow(this);
  _lowmain = new LowMainWindow(this);

  _upmain->setGeometry(QRect(0,0,_upmain->width(),_upmain->height()));
  _lowmain->setGeometry(QRect(0,440,_lowmain->width(),_lowmain->height()));

}
