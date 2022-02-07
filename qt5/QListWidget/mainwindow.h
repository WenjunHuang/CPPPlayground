#pragma once

#include <QWidget>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:

    void on_readDataButton_clicked();

private:
    Ui::MainWindow *ui;
    QStringList _items;
};

