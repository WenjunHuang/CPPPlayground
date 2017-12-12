//
// Created by xxzyjy on 11/12/2017.
//

#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include "AddressWidget.h"
#include "TableModel.h"
#include "NewAddressTab.h"
#include "AddDialog.h"

AddressWidget::AddressWidget(QWidget *parent) : QTabWidget(parent) {
  table = new TableModel(this);
  newAddressTab = new NewAddressTab(this);
  connect(newAddressTab, &NewAddressTab::sendDetails, this, &AddressWidget::addEntry);

  addTab(newAddressTab, "Address Book");
  setupTabs();
}

void AddressWidget::readFromFile(const QString &fileName) {

}

void AddressWidget::writeToFile(const QString &fileName) {

}

void AddressWidget::showAddEntryDialog() {
  AddDialog aDialog;
  if (aDialog.exec()) {
    auto name = aDialog.nameText->text();
    auto address = aDialog.addressText->toPlainText();

    addEntry(name, address);
  }
}

void AddressWidget::addEntry(QString name, QString address) {
  if (!table->getContacts().contains({name, address})) {
    table->insertRows(0, 1, QModelIndex());

    auto index = table->index(0, 0, QModelIndex());
    table->setData(index, name, Qt::EditRole);
    index = table->index(0, 1, QModelIndex());
    table->setData(index, address, Qt::EditRole);
    removeTab(indexOf(newAddressTab));
  } else {
    QMessageBox::information(this, tr("Duplicate Name"), tr("The name \"%1\" already exists").arg(name));
  }
}

void AddressWidget::editEntry() {
  auto *temp = qobject_cast<QTableView *>(currentWidget());
  auto *proxy = qobject_cast<QSortFilterProxyModel *>(temp->model());
  auto *selectionModel = temp->selectionModel();

  QString name;
  QString address;
  int row = -1;
  auto indexes = selectionModel->selectedRows();
    foreach(auto index, indexes) {
      row = proxy->mapToSource(index).row();
      auto nameIndex = table->index(row, 0, QModelIndex());
      auto varName = table->data(nameIndex, Qt::DisplayRole);
      name = varName.toString();

      auto addressIndex = table->index(row, 1, QModelIndex());
      auto varAddr = table->data(addressIndex, Qt::DisplayRole);
      address = varAddr.toString();
    }

  AddDialog aDialog;
  aDialog.setWindowTitle(tr("Edit a Contact"));
  aDialog.nameText->setReadOnly(true);
  aDialog.nameText->setText(name);
  aDialog.addressText->setText(address);

  if (aDialog.exec()) {
    auto newAddress = aDialog.addressText->toPlainText();
    if (newAddress != address) {
      auto index = table->index(row, 1, QModelIndex());
      table->setData(index, newAddress, Qt::EditRole);
    }
  }
}

void AddressWidget::removeEntry() {
  auto temp = qobject_cast<QTableView *>(currentWidget());
  auto proxy = qobject_cast<QSortFilterProxyModel *>(temp->model());
  auto selectionModel = temp->selectionModel();
  auto indexes = selectionModel->selectedRows();

    foreach(QModelIndex index, indexes) {
      int row = proxy->mapToSource(index).row();
      table->removeRows(row, 1, QModelIndex());
    }

  if (table->rowCount(QModelIndex()) == 0) {
    insertTab(0, newAddressTab, "Address Book");
  }
}


void AddressWidget::setupTabs() {
  QStringList groups;
  groups << "ABC" << "DEF" << "GHI" << "JKL" << "MNO" << "PQR" << "STU" << "VW" << "XYZ";
    foreach(auto str, groups) {
      auto regExp = QString("^[%1].*").arg(str);
      auto proxyModel = new QSortFilterProxyModel(this);
      proxyModel->setSourceModel(table);
      proxyModel->setFilterRegExp(QRegExp(regExp));
      proxyModel->setFilterKeyColumn(0);

      auto tableView = new QTableView;
      tableView->setModel(proxyModel);
      tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
      tableView->verticalHeader()->hide();
      tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
      tableView->setSelectionMode(QAbstractItemView::SingleSelection);
      tableView->setSortingEnabled(true);

      connect(tableView->selectionModel(),
              &QItemSelectionModel::selectionChanged,
              this,
              &AddressWidget::selectionChanged);
      connect(this, &QTabWidget::currentChanged, this, [this](int tabIndex) {
        auto *tableView = qobject_cast<QTableView *>(widget(tabIndex));
        if (tableView)
          emit selectionChanged(tableView->selectionModel()->selection());

      });
      addTab(tableView, str);
    }
}
