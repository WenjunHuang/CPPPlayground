#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "person_model.h"

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);
  _model = new PersonModel{this};

  ui->listView->setModel(_model);
  ui->tableView->setModel(_model);
  ui->treeView->setModel(_model);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_addPersonButton_clicked() {
  bool ok;
  auto name =

}

