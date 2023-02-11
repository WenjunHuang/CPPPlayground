//
// Created by rick on 2022/3/2.
//

#include "star_editor.h"
#include <QPainter>
#include <QMouseEvent>

void StarEditor::mouseReleaseEvent(QMouseEvent *event) {
  Q_UNUSED(event);
  emit editingFinished();
}

void StarEditor::mouseMoveEvent(QMouseEvent *event) {
  int rating = event->x() / 20;
  if (rating != _startRating && rating < 6) {
    _startRating = rating;
    update();
  }
}

void StarEditor::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter{this};
  painter.save();
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);

  painter.setBrush(QBrush{Qt::green});
  painter.drawRect(rect());

  painter.setBrush(QBrush{Qt::yellow});

  painter.translate(rect().x(), rect().y() + 10);
  painter.scale(0.1, 0.1);

  for (int i = 0; i < _startRating; i++) {
    painter.drawPolygon(_poly);
    painter.translate(220, 0);
  }

  painter.restore();
}

StarEditor::StarEditor(QWidget *parent) : QWidget{parent} {
  setMouseTracking(true);
  _poly << QPoint{0, 85}
        << QPoint{75, 75}
        << QPoint{100, 10}
        << QPoint{125, 75}
        << QPoint{200, 85}
        << QPoint{150, 125}
        << QPoint{160, 190}
        << QPoint{100, 150}
        << QPoint{40, 190}
        << QPoint{50, 125}
        << QPoint{0, 85};

}

int StarEditor::getStartRating() const {
  return _startRating;
}

void StarEditor::setStartRating(int startRating) {
  _startRating = startRating;
}

QSize StarEditor::sizeHint() const {
  return {100, 50};
}
