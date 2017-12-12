//
// Created by xxzyjy on 11/12/2017.
//

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>

#include "NewAddressTab.h"
#include "AddDialog.h"

NewAddressTab::NewAddressTab(QWidget *parent) : QWidget(parent) {
  auto descriptionLabel = new QLabel(tr("There are currently no contacts in your address book."
                                          "\nClick Add to add new contacts."));
  auto addButton = new QPushButton(tr("Add"));

  connect(addButton, &QAbstractButton::clicked, this, &NewAddressTab::addEntry);

  auto mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(descriptionLabel);
  mainLayout->addWidget(addButton, 0, Qt::AlignCenter);
}

void NewAddressTab::addEntry() {
  AddDialog aDialog;

  if (aDialog.exec()){
    auto name = aDialog.nameText->text();
    auto address = aDialog.addressText->toPlainText();

    emit sendDetails(name,address);
  }

}
