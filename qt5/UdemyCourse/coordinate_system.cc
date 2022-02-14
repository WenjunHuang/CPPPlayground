//
// Created by rick on 2022/1/27.
//
#include <QApplication>
#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QDebug>

class Widget : public QWidget {
Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr) : QWidget{parent} {}

protected:
    void paintEvent(QPaintEvent *event) override {
      QPainter painter{this};

      QBrush brush;
      brush.setColor(Qt::red);
      brush.setStyle(Qt::SolidPattern);

      // First row
      painter.setBrush(brush);
      painter.drawRect(20,20,100,100);

      brush.setColor(Qt::blue);
      brush.setStyle(Qt::Dense1Pattern);
      painter.setBrush(brush);
      painter.drawRect(130,20,100,100);

      brush.setColor(Qt::red);
      brush.setStyle(Qt::Dense2Pattern);
      painter.setBrush(brush);
      painter.drawRect(240,20,100,100);

      brush.setColor(Qt::black);
      brush.setStyle(Qt::Dense3Pattern);
      painter.setBrush(brush);
      painter.drawRect(350,20,100,100);
    }
};

int main(int argc, char *argv[]) {
  QApplication app{argc, argv};
  Widget widget;
  widget.show();
  return QApplication::exec();
}

#include "coordinate_system.moc"

