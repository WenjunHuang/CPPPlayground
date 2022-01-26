//
// Created by rick on 2022/1/17.
//

#pragma once

#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:

    void onClick();

private:
    QPushButton *button;

};


