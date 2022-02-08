#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPalette>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);

  _colorList = QColor::colorNames();
  _model = new QStringListModel{_colorList, this};
  _proxyModel = new QSortFilterProxyModel{this};
  _proxyModel->setSourceModel(_model);

  ui->listView->setModel(_proxyModel);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_listView_clicked(const QModelIndex &index) {
  QPalette palette{ui->label->palette()};
//  palette.setCurrentColorGroup(QPalette::Normal);
  qDebug() << _proxyModel->data(index, Qt::DisplayRole).toString();
  palette.setColor(QPalette::Window, QColor{_proxyModel->data(index, Qt::DisplayRole).toString()});
  ui->label->setPalette(palette);
}


void MainWindow::on_matchStringLineEdit_textChanged(const QString &arg1) {
  _proxyModel->setFilterRegExp(ui->matchStringLineEdit->text());
}

