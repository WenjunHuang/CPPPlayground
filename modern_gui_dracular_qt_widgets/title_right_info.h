//
// Created by rick on 2021/4/11.
//

#pragma once

#include <QLabel>
#include <QtWidgets>

class TitleRightInfo : public QLabel {
    Q_OBJECT
  public:
    TitleRightInfo(QWidget* parent = nullptr);

    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

    void setWindowToMove(QMainWindow* );
  private:
    QMainWindow* _windowToMove = nullptr;
    QPoint _dragPos;
    bool _dragging;
};
