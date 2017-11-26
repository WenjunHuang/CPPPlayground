//
// Created by xxzyjy on 25/11/2017.
//

#include <QLabel>
#include "CsxfWidget.h"
#include "PositionableWidget.h"
#include "ThreeStateButton.h"

CsxfWidget::CsxfWidget(QWidget *parent) : AbstractMainWidget(parent) {
  auto logo = new PositionableWidget<QLabel>(topWidget());
  logo->setObjectName("logoLabel");

  auto title = new PositionableWidget<QLabel>(topWidget());
  title->setText(tr("您的电脑很安全！"));
  title->setObjectName("titleLabel");

  title = new PositionableWidget<QLabel>(topWidget());
  title->setText(tr("定期木马查杀,漏洞修复,时刻保持电脑健康"));
  title->setObjectName("titleLabel2");

  auto startScanBtn = new PositionableWidget<ThreeStateButton>(topWidget());
  startScanBtn->setObjectName("startScanBtn");

//  auto quickScanBtn = new PositionableWidget<
}
