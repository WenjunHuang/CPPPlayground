#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->treeWidget->setColumnCount(2);

  auto googleRoot = new QTreeWidgetItem{ui->treeWidget};
  googleRoot->setText(0, "Google Inc");
  googleRoot->setText(1, "Head Quarters");

  auto googleIndia = new QTreeWidgetItem{};
  googleIndia->setText(0, "Google India");
  googleIndia->setText(1, "Google India Branch");
  googleRoot->addChild(googleIndia);
}

MainWindow::~MainWindow() {
  delete ui;
}


void MainWindow::newPerson() {

}

