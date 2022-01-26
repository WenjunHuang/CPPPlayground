//
// Created by HUANG WEN JUN on 2022/1/5.
//
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <array>

class Example : public QWidget {
public:
    explicit Example(QWidget *parent = nullptr) : QWidget{parent} {
        initUI();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter{this};
        drawLines(painter);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            auto x = event->x();
            auto y = event->y();

            cs_[count_][0] = x;
            cs_[count_][1] = y;
            count_++;
            repaint();
        }

        if (event->button() == Qt::RightButton) {
            repaint();
            count_ = 0;
        }

    }

private:
    void initUI() {
        count_ = 0;
        setGeometry(300, 300, 350, 250);
        setWindowTitle("Lines");
        show();
    }

    void drawLines(QPainter &painter) {
        painter.setRenderHint(QPainter::Antialiasing);
        auto w = width();
        auto h = height();
        painter.eraseRect(0, 0, w, h);

        painter.setPen(QColor::fromRgb(255,0,0));
        for (int i = 0; i < count_; ++i) {
            for (int j = 0; j < count_; ++j) {
                painter.drawLine(cs_[i][0],
                                 cs_[i][1],
                                 cs_[j][0],
                                 cs_[j][1]);
            }
        }
    }

    std::array<std::array<int32_t, 2>, 100> cs_;
    int32_t count_;
};

int main(int argc, char *argv[]) {
    QApplication app{argc, argv};
    Example win;
    return QApplication::exec();
}

#include "Lines.moc"