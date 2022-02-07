//
// Created by rick on 2022/2/7.
//

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>

class Widget : public QWidget {
Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr) : QWidget{parent} {}

protected:
    void paintEvent(QPaintEvent *event) override {
      QPainter painter{this};
      QPainterPath path{};
      path.addRect(100, 100, 100, 100);
      path.moveTo(150, 150);
      path.lineTo(150, 50);
      path.arcTo(50, 50, 200, 200, 90, 90);
      path.lineTo(150,150);

      painter.setBrush(QColor{0x50fa7b});
      painter.drawPath(path);

      QPainterPath path2{};
      path2.addEllipse(100,220,100,100);
      path2.addEllipse(400,220,100,100);
      path2.moveTo(150,220);
      path2.lineTo(450,220);
      path2.moveTo(150,320);
      path2.lineTo(450,320);

      painter.drawPath(path2);
    }
};

int main(int argc, char *argv[]) {
  QApplication app{argc, argv};
  auto window = new Widget{};
  window->setBaseSize(640, 480);
  window->show();
  return QApplication::exec();
}

#include "QPainterPath.moc"