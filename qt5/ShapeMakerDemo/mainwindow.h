#pragma once

#include <QWidget>
#include "shape_canvas.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_shapeCombo_activated(int index);

    void on_penWidth_valueChanged(int arg1);

    void on_penStyle_activated(int index);

    void on_penCap_activated(int index);

    void on_penJoin_activated(int index);

    void on_brushStyle_activated(int index);

    void on_antialising_toggled(bool checked);

    void on_transforms_toggled(bool checked);

    void penChanged();
    void brushChanged();

private:
    Ui::MainWindow *ui;
    ShapeCanvas *_canvas;
};

