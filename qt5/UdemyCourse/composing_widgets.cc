//
// Created by rick on 2022/1/26.
//
#include <QApplication>
#include <QColor>
#include <QList>
#include <QtWidgets/QtWidgets>
#include <algorithm>

class ColorPicker : public QWidget {
Q_OBJECT
public:
    explicit ColorPicker(QWidget *parent = nullptr) : QWidget{parent} {
      populateColors();
      setupUi();
    }

    [[nodiscard]] QColor getColor() const {
      return color_;
    }

    void setColor(const QColor &color) {
      color_ = color;
    }

signals:

    void colorChanged(QColor newColor);

private:
    void populateColors() {
      colorList_ << Qt::red << Qt::green << Qt::blue
                 << Qt::cyan << Qt::darkRed << Qt::darkGray
                 << Qt::gray << Qt::yellow << Qt::darkYellow;
    }

    void setupUi() {
      auto vLayout = new QVBoxLayout{this};
      gLayout_ = new QGridLayout{};

      QSizePolicy policy{QSizePolicy::Minimum, QSizePolicy::Expanding};
      std::initializer_list<std::pair<QString, int>> buttons = {{"one",   0},
                                                                {"two",   1},
                                                                {"three", 2},
                                                                {"four",  3},
                                                                {"five",  4},
                                                                {"six",   5},
                                                                {"seven", 6},
                                                                {"eight", 7},
                                                                {"nine",  8}};
      std::for_each(buttons.begin(),
                    buttons.end(),
                    [=, index = 0](auto &item) mutable {

                        auto button = new QPushButton{item.first, this};
                        button->setSizePolicy(policy);
                        button->setStyleSheet(QString("background-color: %1")
                                                .arg(colorList_.at(item.second).name()));
                        connect(button, &QPushButton::clicked, [=]() {
                            label_->setStyleSheet(QString("background-color: %1").arg(colorList_.at(index).name()));
                            emit colorChanged(colorList_.at(index));
                        });
                        auto column = index % 3;
                        auto row = index / 3;
                        gLayout_->addWidget(button, row, column);

                        index++;
                    });

      label_ = new QLabel{"Color"};
      label_->setStyleSheet(QString("background-color: #eeeab6"));
      label_->setFixedHeight(90);

      vLayout->addWidget(label_);
      vLayout->addLayout(gLayout_);
    }

    QColor color_;
    QList<QColor> colorList_;
    QGridLayout *gLayout_;
    QLabel *label_;
};

int main(int argc, char *argv[]) {
  QApplication app{argc,argv};
  ColorPicker window;
  window.show();

  return app.exec();
}

#include "composing_widgets.moc"