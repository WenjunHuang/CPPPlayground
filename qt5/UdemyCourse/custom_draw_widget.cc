//
// Created by rick on 2022/1/27.
//
#include <QtWidgets/QtWidgets>
#include <QApplication>
#include <variant>
#include <optional>
#include <QTimer>

class WaterTank : public QWidget {
Q_OBJECT
public:
    explicit WaterTank(double maxCapacity, QWidget *parent = nullptr) : QWidget{parent}, maxCapacity_{maxCapacity},
                                                                        current_{50.0} {
      assert(maxCapacity > 0);
      setSizePolicy({QSizePolicy::Preferred, QSizePolicy::Preferred});
    }

    QSize sizeHint() const override {
      return {300, 300};
    }

protected:
    void paintEvent(QPaintEvent *event) override {
      QPen pen{};
      pen.setWidth(3);
      pen.setColor(Qt::black);
      QPainter painter{this};
      painter.setPen(pen);

      painter.drawLine(0, 0, 0, 300);
      painter.drawLine(0, 300, 300, 300);
      painter.drawLine(300, 0, 300, 300);

      auto height = 300 * (1.0 - (current_ / maxCapacity_));
      painter.setBrush(Qt::blue);
      painter.drawRect(0, height, 300, 300);
    }

private:
    double maxCapacity_;
    double current_;
};

class Indicator : public QWidget {
Q_OBJECT
public:
    explicit Indicator(QWidget *parent = nullptr) : QWidget{parent}, active_{ActiveLight::Yellow}, lightOn_{false} {
      setSizePolicy({QSizePolicy::Preferred,
                     QSizePolicy::Preferred});
      timer_ = new QTimer{this};
      timer_->setInterval(700);
      connect(timer_, &QTimer::timeout, this, &Indicator::toggleLights);
      timer_->start();
    }

public slots:

    void activateNormal() {
      active_ = ActiveLight::Green;
    }

    void activateWarning() {
      active_ = ActiveLight::Yellow;

    }

    void activateDanger() {
      active_ = ActiveLight::Red;

    }

protected:
    void paintEvent(QPaintEvent *event) override {
      QPen pen{};
      pen.setWidth(3);
      pen.setColor(Qt::black);

      QPainter painter{this};
      painter.setPen(pen);
      painter.setBrush(Qt::gray);

      painter.drawRect(QRectF{0, 0, 120, 350});

      // draw the three lights
      painter.setBrush(lightOn_ && active_ == ActiveLight::Red ? Qt::red : Qt::black);
      painter.drawEllipse(10, 10, 100, 100);

      painter.setBrush(lightOn_ && active_ == ActiveLight::Green ? Qt::green : Qt::black);
      painter.drawEllipse(10, 115, 100, 100);

      painter.setBrush(lightOn_ && active_ == ActiveLight::Yellow ? Qt::yellow : Qt::black);
      painter.drawEllipse(10, 220, 100, 100);
    }

public:
    QSize sizeHint() const override {
      return {120, 350};
    }

private:
    void toggleLights() {
      lightOn_ = !lightOn_;
      update();

    }

    enum class ActiveLight {
        Green, Red, Yellow
    };
    ActiveLight active_;
    bool lightOn_;
    QTimer *timer_;
};

class Widget : public QWidget {
Q_OBJECT
public:
    Widget(QWidget *parent = nullptr) : QWidget{parent} {
      setSizePolicy({QSizePolicy::Preferred, QSizePolicy::Preferred});
      auto layout = new QHBoxLayout{this};
      auto indicator = new Indicator{};
      auto tank = new WaterTank{100.0f};
      layout->setSpacing(24);
      layout->addWidget(tank);
      layout->addWidget(indicator);
    }
};

int main(int argc, char *argv[]) {
  QApplication app{argc, argv};
  Widget widget;
  widget.show();
  return app.exec();
}

#include "custom_draw_widget.moc"