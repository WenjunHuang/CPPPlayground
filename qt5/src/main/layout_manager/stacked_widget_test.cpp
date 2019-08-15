//
// Created by xxzyjy on 26/10/2017.
//
#include "catch.hpp"
#include "src/main/include/fixture.h"
#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QHBoxLayout>

class StackDlg : public QDialog {
public:
  StackDlg(QWidget *parent = nullptr) : QDialog(parent) {
    setWindowTitle(QObject::tr("StackedWidgetk"));
    _list = new QListWidget(this);
    _list->insertItem(0, tr("Window1"));
    _list->insertItem(1, tr("Window2"));
    _list->insertItem(2, tr("Window3"));

    _label1 = new QLabel(tr("WindowTest1"));
    _label2 = new QLabel(tr("WindowTest2"));
    _label3 = new QLabel(tr("WindowTest3"));

    _stack = new QStackedWidget(this);
    _stack->addWidget(_label1);
    _stack->addWidget(_label2);
    _stack->addWidget(_label3);

    auto mainLayout = new QHBoxLayout();
    mainLayout->setMargin(5);
      mainLayout->setSpacing(5);
      mainLayout->addWidget(_list);
      mainLayout->addWidget(_stack,0,Qt::AlignHCenter);
    mainLayout->setStretchFactor(_list,1);
    mainLayout->setStretchFactor(_stack,3);
    setLayout(mainLayout);

    connect(_list,&QListWidget::currentRowChanged,_stack,&QStackedWidget::setCurrentIndex);

  }

private:
  QListWidget *_list;
  QStackedWidget *_stack;
  QLabel *_label1;
  QLabel *_label2;
  QLabel *_label3;

};

TEST_CASE_METHOD(TestFixture, "stacked_widget", "[QStackedWidget]") {
    auto dlg = new StackDlg;

    dlg->show();
    application.exec();
}

