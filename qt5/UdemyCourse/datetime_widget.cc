//
// Created by rick on 2022/1/27.
//
#include <QtWidgets/QtWidgets>

class DateTimeWidget : public QWidget {
Q_OBJECT
public:
    explicit DateTimeWidget(QWidget *parent = nullptr) : QWidget{parent} {
      auto layout = new QVBoxLayout{this};
      QFont font{"Consolas", 20, QFont::Bold};
      QSizePolicy policy{QSizePolicy::Expanding, QSizePolicy::Fixed};

      dateString_ = QDate::currentDate().toString(Qt::TextDate);
      labelTop_ = new QLabel{this};
      labelTop_->setText(dateString_);
      labelTop_->setFont(font);
      labelTop_->setAlignment(Qt::AlignCenter);
      labelTop_->setSizePolicy(policy);

      timeString_ = QTime::currentTime().toString();
      labelBottom_ = new QLabel{this};
      labelBottom_->setText(timeString_);
      labelBottom_->setFont(font);
      labelBottom_->setAlignment(Qt::AlignCenter);
      labelBottom_->setSizePolicy(policy);
      labelBottom_->setStyleSheet(QString("background-color:#00eff9;color:#fffff1"));

      layout->addWidget(labelTop_);
      layout->addWidget(labelBottom_);
      setLayout(layout);
      setSizePolicy(policy);

      // setup timer
      timer_ = new QTimer{this};
      timer_->setInterval(1000);
      connect(timer_, &QTimer::timeout, this, &DateTimeWidget::updateTime);
      timer_->start();
    }

public slots:

    void updateTime() {
      labelBottom_->setText(QTime::currentTime().toString());
    };

private:
    QTimer *timer_;
    QLabel *labelTop_;
    QLabel *labelBottom_;
    QString dateString_;
    QString timeString_;

};

int main(int argc, char *argv[]) {
  QApplication app{argc, argv};
  DateTimeWidget *widget = new DateTimeWidget{};
  widget->show();
  return QApplication::exec();
}

#include "datetime_widget.moc"