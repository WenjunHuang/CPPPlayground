//
// Created by xxzyjy on 26/10/2017.
//
#include "catch.hpp"
#include <QtWidgets>

class TestsFixture {
public:
  int argc = 1;
  char *argv[1] = {"app"};

  QApplication *app;
  QVBoxLayout *layout;
  QWidget *window;

  TestsFixture(){
    app = new QApplication(argc,argv);
    layout = new QVBoxLayout;
    window = new QWidget;
  }
};

TEST_CASE_METHOD(TestsFixture,"tree_widget","[QTree]"){
  auto label = new QLabel(window);
  label->setText("hello QTreeWidget");

  auto treeWidget = new QTreeWidget(window);
  auto group1 = new QTreeWidgetItem(treeWidget);
  group1->setText(0,"group1");
  group1->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
  group1->setCheckState(0,Qt::Unchecked);

  auto subItem1 = new QTreeWidgetItem(group1);
  subItem1->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
  subItem1->setText(0,"subItem1");
  subItem1->setCheckState(0,Qt::Unchecked);

  auto subItem2 = new QTreeWidgetItem(group1);
  subItem2->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
  subItem2->setText(0,"subItem2");
  subItem2->setCheckState(0,Qt::Unchecked);

  app->connect(treeWidget,&QTreeWidget::currentItemChanged,[label](auto current, auto previous){
    auto str = current->text(0);
    qDebug() << str;
    label->setText(str);
  });

  layout->addWidget(label);
  layout->addWidget(treeWidget);

  window->setLayout(layout);
  window->show();
  app->exec();
}
