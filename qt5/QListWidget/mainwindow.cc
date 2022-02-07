#include <QListWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->listWidget->setIconSize(QSize{70, 70});

  _items << "material_poisondragon_teeth"
         << "material_polymer_1_blue"
         << "material_polymer_1_orange"
         << "material_polymer_2_orange";

  std::for_each(_items.cbegin(), _items.cend(), [this](auto &itemName) {
      auto item = new QListWidgetItem{itemName, ui->listWidget};
      item->setIcon(QIcon{QString{":/images/%1.png"}.arg(itemName)});
      item->setData(Qt::UserRole, itemName);
      item->setData(Qt::DisplayRole, itemName +"Funny");
  });
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_readDataButton_clicked() {
  auto fruit = ui->listWidget->currentItem()->data(Qt::DisplayRole).toString();
  qDebug() << "Current fruit: " << fruit;
  qDebug() << "Current index:" << ui->listWidget->currentIndex().row();
}

