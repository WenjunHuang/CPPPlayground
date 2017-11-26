//
// Created by xxzyjy on 27/10/2017.
//

#include "catch.hpp"
#include "fixture.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QtWidgets>

class InfoDialog:public QDialog{
public:
  InfoDialog(QWidget *parent = nullptr):QDialog(parent){
    setWindowTitle(tr("UserInfo"));

    userNameLabel = new QLabel(tr("用户名:"));
    userNameLineEdit = new QLineEdit;
    nameLabel = new QLabel(tr("姓名:"));
    nameLineEdit = new QLineEdit;
    sexLabel = new QLabel(tr("性别:"));
    sexComboBox = new QComboBox;
    sexComboBox->addItem(tr("女"));
    sexComboBox->addItem(tr("男"));
    departmentLabel = new QLabel(tr("部门:"));
    departmentTextEdit = new QTextEdit;
    ageLabel = new QLabel(tr("年龄:"));
    ageLineEdit = new QLineEdit;
    otherLabel = new QLabel(tr("备注:"));

    otherLabel->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    leftLayout = new QGridLayout;
    leftLayout->addWidget(userNameLabel,0,0);
    leftLayout->addWidget(userNameLineEdit,0,1);
    leftLayout->addWidget(nameLabel,1,0);
    leftLayout->addWidget(nameLineEdit,1,1);
    leftLayout->addWidget(sexLabel,2,0);
    leftLayout->addWidget(sexComboBox,2,1);
    leftLayout->addWidget(departmentLabel,3,0);

    leftLayout->addWidget(departmentTextEdit,3,1);
    leftLayout->addWidget(ageLabel,4,0);
    leftLayout->addWidget(ageLineEdit,4,1);
    leftLayout->addWidget(otherLabel,5,0,1,2);
    leftLayout->setColumnStretch(0,1);
    leftLayout->setColumnStretch(1,3);

    headLabel = new QLabel(tr("头像:"));
    headIconLabel = new QLabel;
    QPixmap icon("postman.png");
    headIconLabel->setPixmap(icon);
    headIconLabel->resize(icon.width(),icon.height());
    updateHeadBtn = new QPushButton(tr("更新"));

    topRightLayout = new QHBoxLayout();
    topRightLayout->setSpacing(20);
    topRightLayout->addWidget(headLabel);
    topRightLayout->addWidget(headIconLabel);
    topRightLayout->addWidget(updateHeadBtn);

    introductionLabel = new QLabel(tr("个人说明:"));
    introductionTextEdit = new QTextEdit;

    //完成右侧布局
    rightLayout = new QVBoxLayout();
    rightLayout->setMargin(10);
    rightLayout->addLayout(topRightLayout);
    rightLayout->addWidget(introductionLabel);
    rightLayout->addWidget(introductionTextEdit);

    //底部
    okBtn = new QPushButton(tr("确定"));
    cancelBtn = new QPushButton(tr("取消"));
    bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(okBtn);
    bottomLayout->addWidget(cancelBtn);

    auto mainLayout = new QGridLayout(this);
    mainLayout->setMargin(15);
    mainLayout->setSpacing(10);
    mainLayout->addLayout(leftLayout,0,0);
    mainLayout->addLayout(rightLayout,0,1);
    mainLayout->addLayout(bottomLayout,1,0,1,2);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);


  }

private:
  QLabel *userNameLabel;
  QLabel *nameLabel;
  QLabel *sexLabel;
  QLabel *departmentLabel;
  QLabel *ageLabel;
  QLabel *otherLabel;
  QLineEdit *userNameLineEdit;
  QLineEdit *nameLineEdit;
  QComboBox *sexComboBox;
  QTextEdit *departmentTextEdit;
  QLineEdit *ageLineEdit;
  QGridLayout *leftLayout;

  // right
  QLabel *headLabel;
  QLabel *headIconLabel;
  QPushButton *updateHeadBtn;
  QHBoxLayout *topRightLayout;
  QLabel *introductionLabel;
  QTextEdit *introductionTextEdit;
  QVBoxLayout *rightLayout;

  QPushButton *okBtn;
  QPushButton *cancelBtn;
  QHBoxLayout *bottomLayout;
};

TEST_CASE_METHOD(TestFixture,"info_dialog","[QDialog]"){
  auto window = new InfoDialog;
//  window->setGeometry(QRect(0,0,640,480));
  window->show();

  application.exec();
}