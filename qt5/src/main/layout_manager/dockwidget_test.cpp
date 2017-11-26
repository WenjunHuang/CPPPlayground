//
// Created by xxzyjy on 26/10/2017.
//

#include "catch.hpp"
#include "src/main/include/fixture.h"
#include <QtWidgets>


TEST_CASE_METHOD(TestFixture,"dockerwidget","[QDockerWidget]"){
  auto main = new QMainWindow;
  auto te = new QTextEdit(main);
  te->setText(QObject::tr("Main Window"));
  te->setAlignment(Qt::AlignCenter);
  main->setCentralWidget(te);

  auto docker = new QDockWidget(main);
  docker->setFeatures(QDockWidget::DockWidgetMovable);
  docker->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

  auto te1 = new QTextEdit;
  te1->setText(QObject::tr("Window1,The dock widget can be moved between docks"));
  docker->setWidget(te1);

  main->addDockWidget(Qt::RightDockWidgetArea,docker);

  auto docker2 = new QDockWidget(QObject::tr("DockWindow2"),main);
  docker2->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);
  auto te2 = new QTextEdit;
  te2->setText(QObject::tr("Window2, The dock widget can be detached from the main window"));
  docker2->setWidget(te2);
  main->addDockWidget(Qt::RightDockWidgetArea,docker2);

  auto dock3 = new QDockWidget(QObject::tr("DockWindow3"),main);
  dock3->setFeatures(QDockWidget::AllDockWidgetFeatures);
  auto te3 = new QTextEdit;
  te3->setText(QObject::tr("Window3,The dock widget can be closed,moved and floated"));
  dock3->setWidget(te3);
  main->addDockWidget(Qt::RightDockWidgetArea,dock3);

  main->show();
  application.exec();


}