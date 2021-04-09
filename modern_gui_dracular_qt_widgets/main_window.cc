//
// Created by rick on 2021/4/9.
//

#include "main_window.h"
#include "ui_mainwindow.h"

void MainWindow::foo() {}
MainWindow::MainWindow(QWidget* parent): QMainWindow(parent),_ui(new Ui::MainWindow) {
    _ui->setupUi(this);
}
