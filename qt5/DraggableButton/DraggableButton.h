//
// Created by rick on 19-3-16.
//

#pragma once

#include <QPushButton>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>

class DraggableButton : public QPushButton {
Q_OBJECT
public:
    DraggableButton(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;

    void dragMoveEvent(QDragMoveEvent *event) override;

    void dragLeaveEvent(QDragLeaveEvent *event) override;

};



