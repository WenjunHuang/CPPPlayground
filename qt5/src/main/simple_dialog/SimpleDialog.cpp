//
// Created by xxzyjy on 26/10/2017.
//

#include "SimpleDialog.h"
#include "ui_simple_dialog.h"
SimpleDialog::SimpleDialog(QWidget *parent) : QDialog(parent),
  ui(new Ui::SimpleDialog)
{
  ui->setupUi(this);
  QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
  ui->lineEdit->setValidator(new QRegExpValidator(regExp));

  connect(ui->okButton,&QPushButton::clicked,this,&QDialog::accept);
  connect(ui->cancelButton,&QPushButton::clicked,this, &QDialog::reject);
  connect(ui->lineEdit,&QLineEdit::textChanged,this,&SimpleDialog::onLineEditTextChanged);
}

SimpleDialog::~SimpleDialog()
{
  delete ui;
}

void SimpleDialog::onLineEditTextChanged() {
  ui->okButton->setEnabled(ui->lineEdit->hasAcceptableInput());

}
