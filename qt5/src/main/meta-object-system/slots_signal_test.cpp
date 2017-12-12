//
// Created by xxzyjy on 29/10/2017.
//
#include <iostream>
#include "catch.hpp"
#include "Counter.h"
#include "Listener.h"
#include "fixture.h"

TEST_CASE("slots_signals","[SlotsSignals]") {
  Counter counter;
  Listener listener;
  QObject::connect(&counter,&Counter::valueChanged,&listener, static_cast<void (Listener::*)(int)>(&Listener::onValue));

  counter.setValue(100);

  REQUIRE(listener._value == 100);
}

TEST_CASE("connect_signal_t0_signal","[SlotsSignals]"){
  Counter a,b;
  Listener l;
  QObject::connect(&a,&Counter::valueChanged,&b,&Counter::valueChanged);
  QObject::connect(&b,&Counter::valueChanged,&l,static_cast<void (Listener::*)(int)>(&Listener::onValue),Qt::UniqueConnection);

  a.setValue(999);
  REQUIRE(l._value == 999);
  std::cout<<b.value()<<std::endl;
}

TEST_CASE("signal_mapper","[SlotsSignals]"){
  Counter a,b;
  Listener l;
  QSignalMapper signalMapper;
  signalMapper.setMapping(&a,QString("a"));
  signalMapper.setMapping(&b,QString("b"));

  QObject::connect(&a,&Counter::changed,&signalMapper,static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
  QObject::connect(&b,&Counter::changed,&signalMapper,static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
  QObject::connect(&signalMapper,SIGNAL(mapped(QString)),&l,SLOT(onValue(QString)));

  a.setValue(999);

  REQUIRE(l._sender == "a");
}

class MyCustomEvent:public QEvent {
public:
  MyCustomEvent():QEvent((QEvent::Type)(QEvent::User+ 1)){}
  ~MyCustomEvent(){
    qDebug() << "destroyed";
  }

};

TEST_CASE_METHOD(TestFixture,"destroyed","[Destroyed]") {
  auto event = new MyCustomEvent;
  application.sendEvent(&application,event);
  qDebug() << "about to quit";
}