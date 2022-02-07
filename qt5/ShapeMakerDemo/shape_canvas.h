//
// Created by rick on 2022/2/7.
//

#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QPen>

class ShapeCanvas : public QWidget {
Q_OBJECT


public:
    enum class Shape {
        Polygon, Rect, RoundedRect,
        Ellipse, Pie, Chord, Text, Pixmap,
    };

    Q_ENUM(Shape)

    explicit ShapeCanvas(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

    QSize sizeHint() const override;

    QSize minimumSizeHint() const override;

public:
    Shape getShape() const {
      return _shape;
    }

    void setShape(Shape shape) {
      _shape = shape;
      update();
    }

    const QBrush &getBrush() const {
      return _brush;
    }

    void setBrush(const QBrush &brush) {
      _brush = brush;
      update();
    }

    const QPen &getPen() const {
      return _pen;
    }

    void setPen(const QPen &pen) {
      _pen = pen;
      update();
    }

    bool isAntialiased() const;

    void setAntialiased(bool antialiased);

    bool isTransformed() const;

    void setTransformed(bool transformed);

    const QPixmap &getPixmap() const;

    void setPixmap(const QPixmap &pixmap);

private:
    Shape _shape;
    QPen _pen;
    QBrush _brush;
    bool _antialiased;
    bool _transformed;
    QPixmap _pixmap;
};
