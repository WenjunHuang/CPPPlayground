//
// Created by xxzyjy on 19/11/2017.
//

#include <QPainter>
#include "TwoStateButton.h"

TwoStateButton::TwoStateButton(QWidget *parent) : QPushButton(parent) {
  setCursor(Qt::PointingHandCursor);
  _enter = false;

  QPixmap borderPix(":/image/btnborder.png");
  _pixmapBorder = borderPix.copy(2 * (borderPix.width() / 3),
                                 0,
                                 borderPix.width() / 3,
                                 borderPix.height());

  QObject::connect(this, SIGNAL(pixmapChanged()), this, SLOT(update()));
}

void TwoStateButton::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  if (_enter) {
    painter.drawPixmap(rect(), _pixmapBorder);
  }

  if (!_pixmap.isNull()) {
    painter.drawPixmap(QRect((rect().width() - _pixmap.width()) / 2,
                             (rect().height() - _pixmap.height()) / 3,
                             _pixmap.width(),
                             _pixmap.height()), _pixmap);

    QFont font;
    QFontMetrics fontMetr(font);
    int textWidth = fontMetr.width(text());

    painter.setPen(QColor(124, 124, 124));
    painter.drawText(QPoint((rect().width() - textWidth) / 2,
                            (rect().height() - _pixmap.height()) / 3 + _pixmap.height() + 20),
                     text());
  }
}

void TwoStateButton::enterEvent(QEvent *) {
  _enter = true;
  update();
}

void TwoStateButton::leaveEvent(QEvent *) {
  _enter = false;
  update();
}
