//
// Created by xxzyjy on 26/10/2017.
//
#include "catch.hpp"
#include "src/main/include/fixture.h"
#include <QtWidgets>
#include <QResizeEvent>


TEST_CASE_METHOD(TestFixture, "layout_manager", "[QSplitter]") {
  QFont font("Times", 12);
  application.setFont(font);

  auto splitterMain = new QSplitter(Qt::Horizontal, nullptr);
  auto textLeft = new QTextEdit(QObject::tr("Left Widget"), splitterMain);
  textLeft->setAlignment(Qt::AlignCenter);

  auto splitterRight = new QSplitter(Qt::Vertical, splitterMain);
  splitterRight->setOpaqueResize(false);
  auto textUp = new QTextEdit(QObject::tr("Top Widget"), splitterRight);
  textUp->setAlignment(Qt::AlignCenter);
  auto textBottom = new QTextEdit(QObject::tr("Bottom Widget"), splitterRight);
  textBottom->setAlignment(Qt::AlignCenter);

  splitterMain->setStretchFactor(1, 1);
  splitterMain->setWindowTitle(QObject::tr("Splitter"));

  splitterMain->show();
  application.exec();
}

class MyLayout : public QStackedLayout {
public:
  MyLayout(int area) {
    this->area = area;
  }

  bool hasHeightForWidth() const override {
    return true;
  }

  int heightForWidth(int w) const override {
    if (w > 0)
      return area / w;
    return -1;
  }

  int area;

};

class ResizeWidget2 : public QWidget {
protected:
  void resizeEvent(QResizeEvent *event) override  {
    qDebug() << "resized " <<event->size();
  }
};

TEST_CASE_METHOD(TestFixture, "layout_manager_and_its_widget", "[LayoutManager]") {
  QWidget widget;

  auto layout = new QVBoxLayout(&widget);
  layout->addWidget(new QPushButton("first"));
  layout->addWidget(new QPushButton("last"));
  widget.show();

  qDebug() << widget.metaObject()->className();

  application.exec();
}

