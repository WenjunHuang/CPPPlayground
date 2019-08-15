//
// Created by xxzyjy on 13/11/2017.
//
#include <QListWidget>
#include <QPointer>
#include <QMainWindow>
#include <QLineEdit>
#include <QTreeWidget>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "catch.hpp"
#include "fixture.h"

TEST_CASE_METHOD(TestFixture, "list_widget", "[QListWidget]") {
  QPointer<QListWidget> list = new QListWidget;

  new QListWidgetItem(QObject::tr("Sycamore"), list);
  new QListWidgetItem(QObject::tr("Chestnut"), list);
  new QListWidgetItem(QObject::tr("Mahogany"), list);

  auto newItem = new QListWidgetItem;
  newItem->setText(QObject::tr("New"));
  newItem->setToolTip(QObject::tr("Tooltip for new"));
  newItem->setStatusTip(QObject::tr("StatusTip for new"));
  newItem->setWhatsThis(QObject::tr("WhatsThis for new"));
  list->insertItem(3, newItem);
  list->sortItems(Qt::AscendingOrder);

  list->show();

  application.exec();
}

TEST_CASE_METHOD(TestFixture, "tree_widgets", "[QTreeWidget]") {
  QPointer<QTreeWidget> treeWidget = new QTreeWidget;
  treeWidget->setColumnCount(2);

  QStringList headers;
  headers << QObject::tr("Subject") << QObject::tr("Default");
  treeWidget->setHeaderLabels(headers);

  auto cities = new QTreeWidgetItem(treeWidget);
  cities->setText(0, QObject::tr("Cities"));
  auto osloItem = new QTreeWidgetItem(cities);
  osloItem->setText(1, QObject::tr("Yes"));

  auto planets = new QTreeWidgetItem(treeWidget, cities);

  treeWidget->show();

  application.exec();
}

TEST_CASE_METHOD(TestFixture, "table_widget", "[QTableWidget]") {
  QMainWindow mainWindow;
  auto centerWidget = new QWidget();

  constexpr int row = 12;
  constexpr int col = 3;
  auto tableWidget = new QTableWidget(row, col);

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      auto newItem = new QTableWidgetItem(QObject::tr("%1").arg(pow(i, j + 1)));
      tableWidget->setItem(i, j, newItem);
    }
  }

  auto valuesHeaderItem = new QTableWidgetItem(QObject::tr("Values"));
  tableWidget->setHorizontalHeaderItem(0, valuesHeaderItem);

  auto result = new QLabel();
  auto calcButton = new QPushButton("Caculate Selected");
  QObject::connect(calcButton, &QPushButton::clicked, [=]() {
    auto selected = tableWidget->selectedItems();
    double total = 0;
    for (auto item : selected) {
      bool ok;
      auto value = item->text().toDouble(&ok);

      if (ok && !item->text().isEmpty()) {
        total += value;
      }
    }

    result->setText(QString("selected total: %1").arg(total));
  });

  auto toBeFind = new QLineEdit;
  auto findButton = new QPushButton("Find");
  QObject::connect(findButton, &QPushButton::clicked, [=]() {
    auto toFind = toBeFind->text();
    auto found = tableWidget->findItems(toFind,Qt::MatchWildcard);

    for (auto item : found) {
      tableWidget->setItemSelected(item,true);
    }
  });

  auto layout = new QVBoxLayout(centerWidget);
  layout->setSpacing(10);
  layout->addWidget(tableWidget);
  layout->addWidget(result);
  layout->addWidget(calcButton);
  layout->addWidget(toBeFind);
  layout->addWidget(findButton);

  mainWindow.setCentralWidget(centerWidget);
  mainWindow.show();
  application.exec();
}
