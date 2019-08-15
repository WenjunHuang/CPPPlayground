//
// Created by xxzyjy on 17/11/2017.
//

#include <QPainter>
#include <QFont>
#include "ScoreLabel.h"

ScoreLabel::ScoreLabel(QWidget *parent) : QLabel(parent) {
  connect(this, SIGNAL(pixmapChanged()), this, SLOT(update()));
}

void ScoreLabel::paintEvent(QPaintEvent *) {
  if (!_pixmap.isNull()) {
    QPixmap pixCoreBorder(":/image/examine_score.png");
    QPainter painter(this);

    painter.drawPixmap(rect(), pixCoreBorder);
    painter.drawPixmap((width() - _pixmap.width()) / 2,
                       (height() - _pixmap.height()) / 2,
                       _pixmap);

    QFont font;
    font.setPixelSize(14);
    painter.setFont(font);
    painter.drawText(width() / 2 + _pixmap.width() / 2 - 5,
                     height() / 2 + _pixmap.height() / 2 - 5,
                     "分");
  }
}
