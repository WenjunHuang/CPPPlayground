//
// Created by rick on 2022/1/27.
//
#include <QtWidgets/QtWidgets>

class DoubleClickableButton : public QPushButton {
Q_OBJECT
public:
    explicit DoubleClickableButton(QWidget *parent = nullptr) : QPushButton{parent} {

    }

signals:

    void doubleClicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override {
      emit doubleClicked();
      QWidget::mouseDoubleClickEvent(event);
    }
};

class Widget : public QWidget {
Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr) : QWidget{parent} {
      auto layout = new QVBoxLayout{this};
      setLayout(layout);

      DoubleClickableButton *button = new DoubleClickableButton{};
      QSizePolicy policy{QSizePolicy::Expanding,QSizePolicy::Fixed};
      layout->addWidget(button);
    }
};

int main(int argc, char *argv[]) {

}

#include "double_clickable_button.moc"