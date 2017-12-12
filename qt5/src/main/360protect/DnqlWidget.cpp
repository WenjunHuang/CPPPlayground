//
// Created by xxzyjy on 02/12/2017.
//

#include <QLabel>
#include "DnqlWidget.h"
#include "PositionableWidget.h"
#include "ThreeStateButton.h"
#include "commons.h"
#include "AnimationButton.h"
#include "CleanMainButton.h"
#include "wobjectimpl.h"
W_OBJECT_IMPL((PositionableWidget<T,S>), template<typename T,typename S>)

DnqlWidget::DnqlWidget(QWidget *parent) : AbstractMainWidget(parent) {
  POS_WIDGET(labLogo,QLabel,topWidget());

  POS_WIDGET(cleanBtn,ThreeStateButton,topWidget());

  POS_LABEL(titleLabel,topWidget(),"已选择6项清理类型");

  POS_BUTTON(classicCleanBtn,bottomWidget(),"经典版电脑清理");
  POS_BUTTON(webchatCleanBtn,bottomWidget(),"微信清理");
  POS_BUTTON(appleDeviceCleanBtn,bottomWidget(),"苹果设备清理");
  POS_BUTTON(systemDriveShrinkBtn,bottomWidget(),"系统盘瘦身");

  POS_BUTTON(findBigFileBtn,bottomWidget(),"查找大文件");
  POS_BUTTON(autoCleanBtn,bottomWidget(),"自动清理");
  POS_BUTTON(recoverBtn,bottomWidget(),"恢复区");

  POS_WIDGET(cleanTrashBtn,AnimationButton,bottomWidget());
  cleanTrashBtn->setCheckable(true);

  POS_WIDGET(cleanTraceBtn,CleanMainButton,bottomWidget());
  cleanTraceBtn->setCheckable(true);

}
