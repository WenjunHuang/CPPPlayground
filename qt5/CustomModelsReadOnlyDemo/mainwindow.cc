#include <QInputDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "person_model.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);
  _model = new PersonModel{this};

  ui->listView->setModel(_model);
  ui->tableView->setModel(_model);
  ui->treeView->setModel(_model);

  ui->tableView->setSelectionModel(ui->listView->selectionModel());
  ui->treeView->setSelectionModel(ui->listView->selectionModel());
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_addPersonButton_clicked() {
  bool ok;
  auto name = QInputDialog::getText(nullptr,
                                    "Names",
                                    tr("Person name:"),
                                    QLineEdit::Normal,
                                    "Type in name", &ok);
  if (ok && !name.isEmpty()) {
    int age = QInputDialog::getInt(nullptr,
                                   "Person Age",
                                   "Age",
                                   20,
                                   15,
                                   120);
    _model->addPerson(name, "blue", age);
  } else {
    QMessageBox::information(nullptr, "Info", "You should provide a name");
  }

}


void MainWindow::on_removePersonButton_clicked() {
  auto index = ui->listView->currentIndex();
  _model->removePerson(index);
}

