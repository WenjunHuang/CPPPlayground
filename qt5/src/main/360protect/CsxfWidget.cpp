//
// Created by xxzyjy on 25/11/2017.
//

#include <QLabel>
#include "CsxfWidget.h"
#include "PositionableWidget.h"
#include "ThreeStateButton.h"
#include "AnimationButton.h"
#include "wobjectimpl.h"
W_OBJECT_IMPL((PositionableWidget<T,S>), template<typename T,typename S>)

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

  auto quickScanBtn = new PositionableWidget<AnimationButton>(bottomWidget());
  quickScanBtn->setObjectName("quickScanBtn");

  auto fullScanBtn = new PositionableWidget<AnimationButton>(bottomWidget());
  fullScanBtn->setObjectName("fullScanBtn");

  auto customScanBtn = new PositionableWidget<AnimationButton>(bottomWidget());
  customScanBtn->setObjectName("customScanBtn");

  auto quickScanLabel = new PositionableWidget<QLabel>(bottomWidget());
  quickScanLabel->setObjectName("quickScanLabel");
  quickScanLabel->setText(tr("快速扫描"));

  auto fullScanLabel = new PositionableWidget<QLabel>(bottomWidget());
  fullScanLabel->setObjectName("fullScanLabel");
  fullScanLabel->setText(tr("全盘扫描"));

  auto customScanLabel = new PositionableWidget<QLabel>(bottomWidget());
  customScanLabel->setObjectName("customScanLabel");
  customScanLabel->setText(tr("自定义扫描"));

  auto normalFixBtn = new PositionableWidget<ThreeStateButton>(bottomWidget());
  normalFixBtn->setObjectName("normalFixBtn");

  auto leakFixBtn = new PositionableWidget<ThreeStateButton>(bottomWidget());
  leakFixBtn->setObjectName("leakFixBtn");

  auto normalFixLabel = new PositionableWidget<QLabel>(bottomWidget());
  normalFixLabel->setObjectName("normalFixLabel");
  normalFixLabel->setText(tr("常规修复"));

  auto leakFixLabel = new PositionableWidget<QLabel>(bottomWidget());
  leakFixLabel->setObjectName("leakFixLabel");
  leakFixLabel->setText(tr("漏洞修复"));

  auto labLabel = new PositionableWidget<QLabel>(bottomWidget());
  labLabel->setObjectName("labLabel");
  labLabel->setText(tr("已启用查杀引擎："));


  for (int i=1;i < 6;i++) {
    auto btnDown1 = new PositionableWidget<ThreeStateButton>(bottomWidget());
    btnDown1->setObjectName(QString("btnDown%1").arg(i));
  }

  auto settingBtn = new PositionableWidget<ThreeStateButton>(bottomWidget());
  settingBtn->setObjectName("settingBtn");
  settingBtn->setText(tr("设置"));

  auto trustyBtn = new PositionableWidget<QPushButton>(bottomWidget());
  trustyBtn->setObjectName("trustyBtn");
  trustyBtn->setText(tr("信任区"));

  auto recoverBtn = new PositionableWidget<QPushButton>(bottomWidget());
  recoverBtn->setObjectName("recoverBtn");
  recoverBtn->setText(tr("恢复区"));

  auto reportBtn = new PositionableWidget<QPushButton>(bottomWidget());
  reportBtn->setObjectName("reportBtn");
  reportBtn->setText(tr("上报区"));
}
