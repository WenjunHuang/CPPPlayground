//
// Created by rick on 2021/4/11.
//

#include <QMouseEvent>
#include "title_right_info.h"

TitleRightInfo::TitleRightInfo(QWidget* parent):QLabel(parent){}
void TitleRightInfo::mouseMoveEvent(QMouseEvent* event) {
    if (_windowToMove != nullptr && _dragging == true) {
        if (event->buttons() == Qt::LeftButton) {
            _windowToMove->move(_windowToMove->pos() + event->globalPos() - _dragPos);
            _dragPos = event->globalPos();
            event->accept();
        }
    } else {
        QLabel::mouseMoveEvent(event);
    }
}
void TitleRightInfo::setWindowToMove(QMainWindow* window) {
    _windowToMove = window;
    _dragPos = QPoint();
}
void TitleRightInfo::mousePressEvent(QMouseEvent* event) {
    if (_windowToMove != nullptr){
        _dragPos = event->globalPos();
        event->accept();
        _dragging = true;
    }
    QLabel::mousePressEvent(event);
}
void TitleRightInfo::mouseReleaseEvent(QMouseEvent* ev) {
    if (_windowToMove != nullptr && _dragging == true) {
            _dragging = false;
            ev->accept();
    }else {
        QLabel::mouseReleaseEvent(ev);
    }
}
