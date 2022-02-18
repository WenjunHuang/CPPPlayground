#include <QInputDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "person_model.h"
#include <QMessageBox>
#include "person_delegate.h"

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);

  auto personDelegate = new PersonDelegate{this};
  _model = new PersonModel{this};

  ui->listView->setModel(_model);
  ui->tableView->setModel(_model);
  ui->tableView->setItemDelegateForColumn(2, personDelegate);
  ui->treeView->setModel(_model);

  auto selectionModel = ui->listView->selectionModel();
  ui->tableView->setSelectionModel(selectionModel);
  ui->treeView->setSelectionModel(selectionModel);
  ui->removePersonButton->setEnabled(!selectionModel->selection().isEmpty());

  connect(selectionModel, &QItemSelectionModel::selectionChanged,
          [this](const QItemSelection &selected, auto &deselected) {
              ui->removePersonButton->setEnabled(!selected.isEmpty());
          });
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

