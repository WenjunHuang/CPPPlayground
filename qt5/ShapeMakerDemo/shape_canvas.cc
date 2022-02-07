//
// Created by rick on 2022/2/7.
//

#include <QPainter>
#include "shape_canvas.h"

ShapeCanvas::ShapeCanvas(QWidget *parent) : QWidget{parent}, _shape{Shape::Polygon},
                                            _antialiased{false},
                                            _transformed{false},
                                            _pixmap{":/images/qt.png"} {}

void ShapeCanvas::paintEvent(QPaintEvent *event) {
  QPainter painter{this};
  // Polygon
  static const QPoint points[] = {{10, 80},
                                  {20, 10},
                                  {80, 30},
                                  {90, 70}};

  QRect rect{10, 20, 80, 60};
  int startAngle = 20 * 16;
  int arcLength = 120 * 16;

  painter.setPen(_pen);
  painter.setBrush(_brush);
  painter.setFont(QFont{"Consolas",8,QFont::Bold});

  if (_antialiased)
    painter.setRenderHint(QPainter::Antialiasing, true);

  for (int x = 0;x<width();x+=100) {
    for (int y = 0;y<height();y+=100) {
      // Save the painter
      painter.save();
      painter.translate(x,y);

      if (_transformed){
        painter.translate(50,50);
        painter.rotate(60.0);
        painter.scale(0.6,0.9);
        painter.translate(-50,-50);
      }

      switch(_shape) {
        case Shape::Polygon:
          painter.drawPolygon(points,4);
          break;
        case Shape::Rect:
          painter.drawRect(rect);
          break;
        case Shape::RoundedRect:
          painter.drawRoundedRect(rect,25,25,Qt::RelativeSize);
          break;
        case Shape::Ellipse:
          painter.drawEllipse(rect);
          break;
        case Shape::Pie:
          painter.drawPie(rect,startAngle,arcLength);
          break;
        case Shape::Chord:
          painter.drawChord(rect,startAngle,arcLength);
          break;
        case Shape::Text:
          painter.drawText(rect,
                           Qt::AlignCenter,
                           tr("Qt GUI"));
          break;
        case Shape::Pixmap:
          painter.drawPixmap(rect,_pixmap);
          break;
      }

      painter.restore();
    }
  }

  painter.setRenderHint(QPainter::Antialiasing, false);
  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::red);
  painter.drawRect(QRect{0, 0, width() - 1, height() - 1});
}

bool ShapeCanvas::isAntialiased() const {
  return _antialiased;
}

void ShapeCanvas::setAntialiased(bool antialiased) {
  _antialiased = antialiased;
  update();
}

bool ShapeCanvas::isTransformed() const {
  return _transformed;
}

void ShapeCanvas::setTransformed(bool transformed) {
  _transformed = transformed;
  update();
}

const QPixmap &ShapeCanvas::getPixmap() const {
  return _pixmap;
}

void ShapeCanvas::setPixmap(const QPixmap &pixmap) {
  _pixmap = pixmap;
  update();
}

QSize ShapeCanvas::sizeHint() const {
  return {500, 300};
}

QSize ShapeCanvas::minimumSizeHint() const {
  return {400, 200};
}
