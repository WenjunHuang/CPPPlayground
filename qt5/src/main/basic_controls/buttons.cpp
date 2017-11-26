//
// Created by xxzyjy on 26/10/2017.
//
#include "catch.hpp"
#include <QtWidgets>


class TestsFixture {
public:
  int argc = 1;
  char *argv[1] = {"app"};

  QApplication *app;
  QHBoxLayout *layout;
  QWidget *window;

  TestsFixture(){
    app = new QApplication(argc,argv);
    layout = new QHBoxLayout;
    window = new QWidget;
  }
};

TEST_CASE_METHOD(TestsFixture,"QPushButton_opts","[QPushButton]") {
  auto quit = new QPushButton("Quit",nullptr);
  quit->setGeometry(QRect(62,40,75,40));
  quit->setFont(QFont("Times",18,QFont::Bold));

  app->connect(quit,&QPushButton::clicked,app,&QApplication::quit);
  quit->show();
  app->exec();
}

TEST_CASE_METHOD(TestsFixture,"date time","[QDateTime]"){
  auto dateLabel = new QLabel;
  auto dateTime = new QDateTime(QDateTime::currentDateTime());
  dateLabel->setText(dateTime->date().toString("yyyy-dd-MM"));
  layout->addWidget(dateLabel);
  window->setLayout(layout);
  window->show();
  app->exec();
}

TEST_CASE_METHOD(TestsFixture,"timer","[QTimer]"){
  auto timeClock = new QTimer;
  auto dateLabel = new QLabel;

  layout->addWidget(dateLabel);
  app->connect(timeClock,&QTimer::timeout,[&](){
    auto dateTime = QDateTime::currentDateTime();
    dateLabel->setText(dateTime.toString("yyyy-dd-MM hh:mm:ss"));
  });

  timeClock->start(1000);

  window->setLayout(layout);
  window->show();
  app->exec();
}

