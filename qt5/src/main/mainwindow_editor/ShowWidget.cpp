//
// Created by xxzyjy on 28/10/2017.
//

#include <QHBoxLayout>
#include "ShowWidget.h"
ShowWidget::ShowWidget(QWidget *parent):QWidget(parent){
  imageLabel = new QLabel;
  imageLabel->setScaledContents(true);
  text = new QTextEdit;

  auto mainLayout = new QHBoxLayout(this);
  mainLayout->addWidget(imageLabel);
  mainLayout->addWidget(text);
}