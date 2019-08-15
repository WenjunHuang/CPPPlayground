//
// Created by xxzyjy on 10/11/2017.
//

#include <QTextStream>
#include <QFile>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <QCoreApplication>
#include <QApplication>
#include "UpMainWindow.h"
#include "ScoreLabel.h"
#include "WordLineButton.h"
#include "MainWindow.h"
#include "PositionableWidget.h"
#include "ui_upmainwindow.h"
#include "wobjectimpl.h"

W_OBJECT_IMPL((PositionableWidget<T,S>), template<typename T,typename S>)

UpMainWindow::UpMainWindow(QWidget *parent) : QWidget(parent) {
  _ui = new Ui::UpMainWindow;
  _ui->setupUi(this);
  // initialize qss
//  QFile qss(":/qss/upmainwindow");
//  if (qss.open(QFile::ReadOnly)) {
//    QTextStream stream(&qss);
//    setStyleSheet(stream.readAll());
//  }

  _mousePressed = false;

  auto logoLabel = new PositionableWidget<QLabel>(this);
  logoLabel->setObjectName("logoLabel");

  auto titleLabel = new PositionableWidget<QLabel>(this);
  titleLabel->setObjectName("titleLabel");
  titleLabel->setText(tr("360安全卫士领航版"));

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


  auto scoreLabel = new PositionableWidget<ScoreLabel>(this);
  scoreLabel->setObjectName("scoreLabel");

  auto titleLabel1 = new PositionableWidget<QLabel>(this);
  titleLabel1->setObjectName("titleLabel1");
  titleLabel1->setText("上次电脑体检良好，请继续保持！");


  auto titleLabel2 = new PositionableWidget<QLabel>(this);
  titleLabel2->setObjectName("titleLabel2");
  titleLabel2->setText("上次体检是1分钟以前,共发现5个问题,已全部处理");

  auto btnHead = new PositionableWidget<HeadButton>(this);
  btnHead->setObjectName("headBtn");

  auto btnWord = new PositionableWidget<QPushButton>(this);
  btnWord->setObjectName("wordBtn");
  btnWord->setText("登录360账号");

  auto btnExamine = new PositionableWidget<ThreeStateButton>(this);
  btnExamine->setObjectName("examineBtn");

  auto btnSecurity = new PositionableWidget<QPushButton>(this);
  btnSecurity->setObjectName("securityBtn");
  btnSecurity->setCursor(Qt::PointingHandCursor);

  auto securityLabel = new PositionableWidget<QLabel>(this);
  securityLabel->setObjectName("securityLabel");
  securityLabel->setText("安全防护中心");

  auto label = new PositionableWidget<QLabel>(this);
  label->setObjectName("quardlineLabel");
//  label->setGeometry(QRect(120, 360, 1, 50));
//  label->setStyleSheet("background:transparent;border-image:url(:/image/guardline.png);");

  auto payForLabel = new PositionableWidget<QLabel>(this);
  payForLabel->setObjectName("payforLabel");
  payForLabel->setText("网购先赔");

  auto btnNetPayFor = new PositionableWidget<QPushButton>(this);
  btnNetPayFor->setObjectName("netPayforBtn");
  btnNetPayFor->setCursor(Qt::PointingHandCursor);


}

UpMainWindow::~UpMainWindow() {
  delete _ui;
}

void UpMainWindow::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    auto topLevels = QApplication::topLevelWidgets();
    QWidget * mainWin = topLevels.takeFirst();
//    for (auto top:topLevels) {
//      mainWin = qobject_cast<AddressBookMainWindow *>(top);
//      if (mainWin)
//        break;
//    }
    if (mainWin) {
      _windowPos = mainWin->pos();
      _mousePos = event->globalPos();
      _mousePressed = true;
    }
  }
}

void UpMainWindow::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton)
    _mousePressed = false;
}

void UpMainWindow::mouseMoveEvent(QMouseEvent *event) {
  if (_mousePressed) {
    auto topLevels = QApplication::topLevelWidgets();
    QWidget * mainWin = topLevels.takeFirst();
    if (mainWin)
      mainWin->move(_windowPos + (event->globalPos() - _mousePos));
  }
}

void UpMainWindow::paintEvent(QPaintEvent *) {
  QStyleOption option;
  option.initFrom(this);

  QPainter painter(this);
  style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
