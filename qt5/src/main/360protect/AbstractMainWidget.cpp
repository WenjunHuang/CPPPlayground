//
// Created by xxzyjy on 25/11/2017.
//

#include <QVBoxLayout>
#include <QtGui>
#include "AbstractMainWidget.h"
#include "PositionableWidget.h"
#include "ThreeStateButton.h"

AbstractMainWidget::AbstractMainWidget(QWidget *parent) : QWidget(parent) {
  _bottomWidget = new BottomWidget(this);
  _bottomWidget->setObjectName("bottomWidget");
  _topWidget = new TopWidget(this);
  _topWidget->setObjectName("topWidget");

  auto vlayout = new QVBoxLayout(this);
  vlayout->addWidget(_topWidget);
  vlayout->addWidget(_bottomWidget);
  vlayout->setSpacing(0);
  vlayout->setContentsMargins(QMargins(0, 0, 0, 0));
  setLayout(vlayout);

  auto btnUpdate = new PositionableWidget<ThreeStateButton>(this);
  btnUpdate->setObjectName("updateBtn");

  auto btnMini = new PositionableWidget<ThreeStateButton>(this);
  btnMini->setObjectName("min");

  auto btnExit = new PositionableWidget<ThreeStateButton>(this);
  btnExit->setObjectName("close");
  connect(btnExit, &QPushButton::clicked, qApp, &QCoreApplication::quit);

  auto btnFeedback = new PositionableWidget<ThreeStateButton>(this);
  btnFeedback->setObjectName("feedback");

  auto btnMenu = new PositionableWidget<ThreeStateButton>(this);
  btnMenu->setObjectName("Menu");

  auto btnSkin = new PositionableWidget<ThreeStateButton>(this);
  btnSkin->setObjectName("Skin");


}
