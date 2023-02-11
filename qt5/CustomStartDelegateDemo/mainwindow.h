#pragma once

#include <QWidget>
#include <vector>

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
    std::vector<std::vector<QVariant>> _data;
};

