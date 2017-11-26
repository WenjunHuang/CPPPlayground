//
// Created by xxzyjy on 28/10/2017.
//

#include "InputDlg.h"
#include <QDialog>
#include <QGridLayout>
#include <QtWidgets>

struct InputDlgPimp {
  QLabel *nameLabel1;
  QLabel *sexLabel1;
  QLabel *ageLabel1;
  QLabel *scoreLabel1;

  QLabel *nameLabel2;
  QLabel *sexLabel2;
  QLabel *ageLabel2;
  QLabel *scoreLabel2;

  QPushButton *nameBtn;
  QPushButton *sexBtn;
  QPushButton *ageBtn;
  QPushButton *scoreBtn;
  QGridLayout *mainLayout;

  InputDlgPimp() {
    nameLabel1 = new QLabel;
    nameLabel1->setText(QObject::tr("姓名:"));
    nameLabel2 = new QLabel;
    nameLabel2->setText(QObject::tr("张三"));
    nameLabel2->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    nameBtn = new QPushButton;
    nameBtn->setText(QObject::tr("修改姓名"));
    sexLabel1 = new QLabel;
    sexLabel1->setText(QObject::tr("性别:"));
    sexLabel2 = new QLabel;
    sexLabel2->setText(QObject::tr("男"));
    sexLabel2->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    sexBtn = new QPushButton;
    sexBtn->setText(QObject::tr("修改年龄"));
    ageLabel1 = new QLabel;
    ageLabel1->setText(QObject::tr("年龄:"));
    ageLabel2 = new QLabel;
    ageLabel2->setText(QObject::tr("21"));
    ageLabel2->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    ageBtn = new QPushButton;
    ageBtn->setText(QObject::tr("修改年龄"));
    scoreLabel1 = new QLabel;
    scoreLabel1->setText(QObject::tr("成绩:"));
    scoreLabel2 = new QLabel;
    scoreLabel2->setText(QObject::tr("80"));
    scoreLabel2->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    scoreBtn = new QPushButton;
    scoreBtn->setText(QObject::tr("修改成绩"));

    mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel1, 0, 0);
    mainLayout->addWidget(nameLabel2, 0, 1);
    mainLayout->addWidget(nameBtn, 0, 2);
    mainLayout->addWidget(sexLabel1, 1, 0);
    mainLayout->addWidget(sexLabel2, 1, 1);
    mainLayout->addWidget(sexBtn, 1, 2);
    mainLayout->addWidget(ageLabel1, 2, 0);
    mainLayout->addWidget(ageLabel2, 2, 1);
    mainLayout->addWidget(ageBtn, 2, 2);
    mainLayout->addWidget(scoreLabel1, 3, 0);
    mainLayout->addWidget(scoreLabel2, 3, 1);
    mainLayout->addWidget(scoreBtn, 3, 2);
    mainLayout->setMargin(15);
    mainLayout->setSpacing(10);
  }
};

InputDlg::InputDlg(QWidget *parent) : QDialog(parent), pimp{std::make_unique<InputDlgPimp>()} {
  setWindowTitle(QObject::tr("标准输入对话框实例"));

  setLayout(pimp->mainLayout);
  connect(pimp->nameBtn,&QPushButton::clicked,this,&InputDlg::ChangeName);
  connect(pimp->sexBtn,&QPushButton::clicked,this,&InputDlg::ChangeSex);
  connect(pimp->ageBtn,&QPushButton::clicked,this,&InputDlg::ChangeAge);
  connect(pimp->scoreBtn,&QPushButton::clicked,this,&InputDlg::ChangeScore);
}

void InputDlg::ChangeName() {

}

void InputDlg::ChangeSex() {

}

void InputDlg::ChangeAge() {

}

void InputDlg::ChangeScore() {

}

InputDlg::~InputDlg() = default;