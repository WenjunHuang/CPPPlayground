#pragma once

#include <QWidget>
#include "person_model.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PersonModel *_model;

    void on_addPersonButton_clicked();
};

