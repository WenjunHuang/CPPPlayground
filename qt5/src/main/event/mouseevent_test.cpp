//
// Created by xxzyjy on 02/11/2017.
//
#include <QPushButton>
#include "catch.hpp"
#include "fixture.h"
#include "MouseEvent.h"
#include "EventParent.h"
#include "EventChild.h"

TEST_CASE_METHOD(TestFixture,"mouseevent","[MouseEvent]"){
  MouseEvent window;
  window.show();
  application.exec();
}

TEST_CASE_METHOD(TestFixture,"event_propogation","[Event]"){
  const QEvent::Type MyEvent = (QEvent::Type)1234;
  EventParent *parent = new EventParent;
  EventChild *child=new EventChild(parent);

  QPushButton button("Click me");
  QObject::connect(&button, &QPushButton::clicked,[=](){
    QApplication::postEvent(child,new QEvent(MyEvent));
  });

  button.show();
  application.exec();
}


