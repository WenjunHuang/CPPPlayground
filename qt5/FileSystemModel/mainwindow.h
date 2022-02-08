#pragma once

#include <QWidget>
#include <QFileSystemModel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:


    void on_removeButton_clicked();

    void on_addDirButton_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *_model;
};

