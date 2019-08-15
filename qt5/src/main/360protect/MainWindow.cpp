//
// Created by xxzyjy on 05/11/2017.
//

#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QSettings>
#include <QParallelAnimationGroup>
#include "MainWindow.h"
#include "CsxfWidget.h"
#include "PositionableWidget.h"
#include "MaskMainWidget.h"
#include "wobjectimpl.h"

W_OBJECT_IMPL((PositionableWidget<T,S>), template<typename T,typename S>)

struct MainWindowPrivate {
  MainWindowPrivate(MainWindow *ptr) : q_ptr{ptr} {}

  MainWindow *q_ptr;
  PositionableWidget<MaskMainWidget> *maskMainWidget;
  PositionableWidget<QStackedWidget> *stackedWidget;
  CsxfWidget *csxfWidget;

  Q_DECLARE_PUBLIC(MainWindow)

};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), d_ptr{new MainWindowPrivate(this)} {
  setWindowFlags(Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground, true);

  d_ptr->stackedWidget = new PositionableWidget<QStackedWidget>(this);
  d_ptr->stackedWidget->setObjectName("stackedWidget");

  d_ptr->csxfWidget = new CsxfWidget(this);

  d_ptr->stackedWidget->addWidget(d_ptr->csxfWidget);
  d_ptr->stackedWidget->setCurrentIndex(0);

  d_ptr->maskMainWidget = new PositionableWidget<MaskMainWidget>(this);
  d_ptr->maskMainWidget->setObjectName("maskMainWidget");

  initializeAnimations();

  connect(d_ptr->maskMainWidget->lowmain()->killAndFixBtn(),&QPushButton::clicked,[=](){
   d_ptr->stackedWidget->setCurrentIndex(0);
  });

  auto returnToMain = [=](){
    d_ptr->maskMainWidget->show();
  };
  connect(d_ptr->csxfWidget->topWidget(),&TopWidget::onReturn,returnToMain);

}

void MainWindow::initializeAnimations(){
  Q_D(MainWindow);

  QSettings settings(":/settings/mainwindow", QSettings::IniFormat);

  settings.beginGroup("MaskUpMainClose");
  auto maskUpMainClose = new QPropertyAnimation(d->maskMainWidget->upmain(), "pos");
  maskUpMainClose->setStartValue(settings.value("startValue").toPoint());
  maskUpMainClose->setEndValue(settings.value("endValue").toPoint());
  maskUpMainClose->setDuration(settings.value("duration").toInt());
  settings.endGroup();

  settings.beginGroup("MaskLowMainClose");
  auto maskLowMainClose = new QPropertyAnimation(d->maskMainWidget->lowmain(),"pos");
  maskLowMainClose->setStartValue(settings.value("startValue").toPoint());
  maskLowMainClose->setEndValue(settings.value("endValue").toPoint());
  maskLowMainClose->setDuration(settings.value("duration").toInt());
  settings.endGroup();

  auto animationGroup = new QParallelAnimationGroup(this);
  animationGroup->addAnimation(maskUpMainClose);
  animationGroup->addAnimation(maskLowMainClose);

  connect(d->maskMainWidget->lowmain()->killAndFixBtn(), &QPushButton::clicked, [=]() {
    animationGroup->start();
  });
  connect(animationGroup, &QParallelAnimationGroup::finished,d->maskMainWidget,&QWidget::hide);

  auto maskUpShow = new QPropertyAnimation(d->maskMainWidget->upmain(),"pos");
  settings.beginGroup("MaskUpMainShow");
  maskUpShow->setStartValue(settings.value("startValue").toPoint());
  maskUpShow->setEndValue(settings.value("endValue").toPoint());
  maskUpShow->setDuration(settings.value("duration").toInt());
  settings.endGroup();

  auto maskLowShow = new QPropertyAnimation(d->maskMainWidget->lowmain(),"pos");
  settings.beginGroup("MaskLowMainShow");
  maskLowShow->setStartValue(settings.value("startValue").toPoint());
  maskLowShow->setEndValue(settings.value("endValue").toPoint());
  maskLowShow->setDuration(settings.value("duration").toInt());
  settings.endGroup();

  auto maskShow = new QParallelAnimationGroup(this);
  maskShow->addAnimation(maskUpShow);
  maskShow->addAnimation(maskLowShow);

  auto showAnimation = [=](){
    d->maskMainWidget->show();
    maskShow->start();
  };
  connect(d->csxfWidget->topWidget(),&TopWidget::onReturn,showAnimation);

}

MainWindow::~MainWindow() {}
