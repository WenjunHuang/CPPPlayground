#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);

  QStringList labels;
  labels << "Last Name"
         << "First Name"
         << "Age"
         << "Occupation"
         << "Marital Status";
  ui->tableWidget->setHorizontalHeaderLabels(labels);

  _table.append({"John", "Doe", 32, "Farmer", "Single"});

  std::for_each(_table.cbegin(), _table.cend(), [this, row = 0](auto &item)mutable {
      ui->tableWidget->insertRow(row);

      auto firstName = new QTableWidgetItem{};
      firstName->setText(_table[row].firstName);
      ui->tableWidget->setItem(row,0,firstName);
//      ui->tableWidget->setItem(row, 1,_table[row].firstName);
//      ui->tableWidget->item(row, 2)->setText(QString("%1").arg(_table[row].age));
//      ui->tableWidget->item(row, 3)->setText(_table[row].occupation);
//      ui->tableWidget->item(row, 4)->setText(_table[row].marriageStatus);
  });

}

MainWindow::~MainWindow() {
  delete ui;
}


void MainWindow::newPerson() {

}

