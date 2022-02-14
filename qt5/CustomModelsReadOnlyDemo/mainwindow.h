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

private slots:
    void on_addPersonButton_clicked();
    void on_removePersonButton_clicked();

private:
    Ui::MainWindow *ui;
    PersonModel *_model;

};

