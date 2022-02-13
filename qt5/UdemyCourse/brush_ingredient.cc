//
// Created by rick on 2022/1/28.
//
#include <QApplication>
#include <QDebug>
#include <QWidget>
#include <QPainter>

class Widget : public QWidget {
Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr) : QWidget{parent} {

    }

protected:
    void paintEvent(QPaintEvent *event) override {
      QPainter painter{this};
      QLinearGradient linearGradient{{70, 20},
                                     {70, 170}};
      linearGradient.setColorAt(0,Qt::red);
      linearGradient.setColorAt(1,Qt::yellow);

      QBrush brush{linearGradient};
      painter.setBrush(brush);
      painter.drawRect(20,20,100,300);
    }
};

int main(int argc, char *argv[]) {
  QApplication app{argc,argv};
  Widget widget;
  widget.resize(640,480);
  widget.show();

  return QApplication::exec();

}

#include "brush_ingredient.moc"