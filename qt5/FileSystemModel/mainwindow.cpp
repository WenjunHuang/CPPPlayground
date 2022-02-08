#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);
  _model = new QFileSystemModel{this};
  _model->setReadOnly(false);
  _model->setRootPath(QDir::currentPath());
  ui->treeView->setModel(_model);
  ui->treeView->setAlternatingRowColors(true);

  auto index = _model->index(QDir::currentPath());
  ui->treeView->expand(index);
  ui->treeView->scrollTo(index);
  ui->treeView->resizeColumnToContents(0);
}

MainWindow::~MainWindow() {
  delete ui;
}


void MainWindow::on_removeButton_clicked() {
  auto index = ui->treeView->currentIndex();
  if (!index.isValid())
    return;
  bool ok;
  if (_model->fileInfo(index).isDir()){
    ok = _model->rmdir(index);
  } else {
    ok = _model->remove(index);
  }
  if (!ok)
    QMessageBox::information(this,"Delete",
                             QString("Failed to delete %1").arg(_model->fileName(index)));

}


void MainWindow::on_addDirButton_clicked() {
  auto index = ui->treeView->currentIndex();
  if (!index.isValid())
    return;

  auto dirName = QInputDialog::getText(this, "Create Directory", "Directory name");
  if (!dirName.isEmpty()) {
    if (!_model->mkdir(index, dirName).isValid()) {
      QMessageBox::information(this, "Create Directory",
                               "Failed to create the directory");
    }
  }
}

