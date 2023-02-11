#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);

  _data.push_back({"Beginning Qt C++ GUI Development", "Qt C++ GUI", 2});
  _data.push_back({"Qt Quick and QML For Beginners", "QML", 5});
  _data.push_back({"Qt Quick and QML Intermediate", "QML", 4});
  _data.push_back({"Qt 5 C++ GUI Intermediate", "Qt C++ GUI", 1});

  ui->tableWidget->setRowCount(_data.size());
  ui->tableWidget->setColumnCount(_data[0].size());

  std::for_each(_data.begin(), _data.end(), [this,index = 0](auto &row) mutable {
      auto item0 = new QTableWidgetItem{row[0].toString()};
      auto item1 = new QTableWidgetItem{row[1].toString()};
      auto item2 = new QTableWidgetItem{row[2].toString()};

      ui->tableWidget->setItem(index,0,item0);
      ui->tableWidget->setItem(index,1,item1);
      ui->tableWidget->setItem(index,2,item2);
      index++;
  });
}

MainWindow::~MainWindow() {
  delete ui;
}
