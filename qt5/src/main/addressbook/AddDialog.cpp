//
// Created by xxzyjy on 11/12/2017.
//

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QGridLayout>
#include "AddDialog.h"

AddDialog::AddDialog(QWidget *parent) : QDialog(parent) {
  auto nameLabel = new QLabel(tr("Name"));
  auto addressLabel = new QLabel(tr("Address"));
  auto okButton = new QPushButton(tr("OK"));
  auto cancelButton = new QPushButton(tr("Cancel"));

  nameText = new QLineEdit;
  addressText = new QTextEdit;

  auto gLayout = new QGridLayout;
  gLayout->setColumnStretch(1, 2);
  gLayout->addWidget(nameLabel, 0, 0);
  gLayout->addWidget(nameText, 0, 1);
  gLayout->addWidget(addressLabel, 1, 0, Qt::AlignLeft | Qt::AlignTop);
  gLayout->addWidget(addressText, 1, 1, Qt::AlignLeft);

  auto buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  gLayout->addLayout(buttonLayout, 2, 1, Qt::AlignRight);

  auto mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(gLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  setWindowTitle(tr("Add a Contact"));
}
