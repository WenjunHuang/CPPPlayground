//
// Created by xxzyjy on 28/10/2017.
//
#include "catch.hpp"
#include "fixture.h"
#include <QtWidgets>
#include <QDialog>
#include <QGridLayout>
#include <boost/optional.hpp>
#include "InputDlg.h"

using namespace boost;

class Dialogs : public QDialog {
public:
  Dialogs() : QDialog(nullptr) {
    fileBtn = new QPushButton;
    fileBtn->setText(tr("文件标准对话框实例"));
    fileLineEdit = new QLineEdit;

    colorBtn = new QPushButton;
    colorBtn->setText(tr("颜色标准对话框实例"));
    colorFrame = new QFrame;
    colorFrame->setFrameShape(QFrame::Box);
    colorFrame->setAutoFillBackground(true);

    fontBtn = new QPushButton;
    fontBtn->setText(tr("字体标准对话框实例"));
    fontLineEdit = new QLineEdit;
    fontLineEdit->setText(tr("Welcome!"));

    inputBtn = new QPushButton(tr("标准输入对话框实例"));

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(fileBtn, 0, 0);
    mainLayout->addWidget(fileLineEdit, 0, 1);
    mainLayout->addWidget(colorBtn, 1, 0);
    mainLayout->addWidget(colorFrame, 1, 1);
    mainLayout->addWidget(fontBtn, 2, 0);
    mainLayout->addWidget(fontLineEdit, 2, 1);
    mainLayout->addWidget(inputBtn,3,0);

    connect(fileBtn, &QPushButton::clicked, [this]() {
      auto s = QFileDialog::getOpenFileName(this, "open file dialog", "/",
                                            "C++ files(*.cpp)::C files(*.c)::Head files(*.h)");
      fileLineEdit->setText(s);
    });


    connect(colorBtn, &QPushButton::clicked, [this]() {
      auto c = QColorDialog::getColor(Qt::blue);
      if (c.isValid()) {
        colorFrame->setPalette(QPalette(c));
      }
    });

    connect(fontBtn, &QPushButton::clicked, [this]() {
      auto ok = false;
      auto f = QFontDialog::getFont(&ok);
      if (ok) {
        fontLineEdit->setFont(f);
      }
    });

    connect(inputBtn,&QPushButton::clicked,[this](){
      inputDlg = new InputDlg(this);
      inputDlg->setModal(true);
      inputDlg->show();
    });

  }

private:
  QPushButton *fileBtn;
  QLineEdit *fileLineEdit;

  QPushButton *colorBtn;
  QFrame *colorFrame;

  QPushButton *fontBtn;
  QLineEdit *fontLineEdit;

  QPushButton *inputBtn;
  InputDlg *inputDlg;

  QGridLayout *mainLayout;
};

TEST_CASE_METHOD(TestFixture, "dialogs", "[QtDialogs]") {
  auto window = new Dialogs;
  window->show();
  application.exec();
}

