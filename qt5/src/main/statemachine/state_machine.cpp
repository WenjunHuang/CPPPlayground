//
// Created by xxzyjy on 29/10/2017.
//
#include "catch.hpp"
#include "fixture.h"
#include "StringTransition.h"
#include <QtCore>
#include <QPushButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>

TEST_CASE_METHOD(TestFixture, "state_machine", "[state_machine]") {
  auto button = new QPushButton("Button to state");

  QStateMachine machine;
  auto s1 = new QState;
  auto s2 = new QState;
  auto s3 = new QState;

  s1->addTransition(button, &QPushButton::clicked, s2);
  s2->addTransition(button, &QPushButton::clicked, s3);
  s3->addTransition(button, &QPushButton::clicked, s1);

  s1->assignProperty(button, "text", "In state s1");
  s2->assignProperty(button, "text", "In state s2");
  s3->assignProperty(button, "text", "In state s3");

  QObject::connect(s3, &QState::entered, button, &QPushButton::showMaximized);
  QObject::connect(s3, &QState::exited, button, &QPushButton::showMinimized);

  machine.addState(s1);
  machine.addState(s2);
  machine.addState(s3);
  machine.setInitialState(s1);

  machine.start();

  button->show();
  application.exec();
}

TEST_CASE_METHOD(TestFixture, "animation_with_state", "[State]") {
  QPushButton *button = new QPushButton("Animated Button");
  button->show();

  QPointer<QStateMachine> machine = new QStateMachine;
  auto state1 = new QState(machine);
  state1->assignProperty(button, "geometry", QRect(0, 0, 100, 30));
  machine->setInitialState(state1);

  auto state2 = new QState(machine);
  state2->assignProperty(button, "geometry", QRect(250, 250, 100, 30));

  auto transition1 = state1->addTransition(button, &QPushButton::clicked, state2);
  transition1->addAnimation(new QPropertyAnimation(button, "geometry"));
  auto transition2 = state2->addTransition(button, &QPushButton::clicked, state1);
  transition2->addAnimation(new QPropertyAnimation(button, "geometry"));

  machine->start();

  application.exec();
}

TEST_CASE_METHOD(TestFixture, "group_state", "[StateMachine]") {
  auto window = new QFrame();
  auto layout = new QVBoxLayout(window);
  auto button1 = new QPushButton("");
  auto button2 = new QPushButton("Quick");
  layout->addWidget(button1);
  layout->addWidget(button2);

  auto machine = new QStateMachine();
  auto s1 = new QState();
  auto s11 = new QState(s1);
  auto s12 = new QState(s1);
  auto s13 = new QState(s1);
  s11->addTransition(button1, &QPushButton::clicked, s12);
  s12->addTransition(button1, &QPushButton::clicked, s13);
  s13->addTransition(button1, &QPushButton::clicked, s11);
  s11->assignProperty(button1, "text", "State 11");
  s12->assignProperty(button1, "text", "State 12");
  s13->assignProperty(button1, "text", "State 13");

  s1->setInitialState(s11);
  machine->addState(s1);

  auto s2 = new QFinalState;
  s1->addTransition(button2, &QPushButton::clicked, s2);
  machine->addState(s2);
  machine->setInitialState(s1);

  // 当处于s12状态时，忽略退出事件
  s12->addTransition(button2, &QPushButton::clicked, s12);

  QObject::connect(machine, &QStateMachine::finished, &application, &QApplication::quit);

  window->show();
  machine->start();
  application.exec();
}

TEST_CASE_METHOD(TestFixture, "history_state", "[StateMachine]") {
  auto window = new QFrame();
  auto layout = new QVBoxLayout(window);
  auto button = new QPushButton();
  auto interruptButton = new QPushButton("interrupt");
  auto quitButton = new QPushButton("quit");

  layout->addWidget(button);
  layout->addWidget(interruptButton);
  layout->addWidget(quitButton);

  QPointer<QStateMachine> machine = new QStateMachine();
  auto s1 = new QState();
  auto s11 = new QState(s1);
  auto s12 = new QState(s1);
  auto s13 = new QState(s1);
  auto s1h = new QHistoryState(s1);
  s11->addTransition(button, &QPushButton::clicked, s12);
  s12->addTransition(button, SIGNAL(clicked()), s13);
  s13->addTransition(button, SIGNAL(clicked()), s11);
  s11->assignProperty(button, "text", "s11");
  s12->assignProperty(button, "text", "s12");
  s13->assignProperty(button, "text", "s13");
  s1->setInitialState(s11);
  machine->addState(s1);
  machine->setInitialState(s1);

  auto s3 = new QState();
  s3->assignProperty(button, "text", "In s3");
  auto mbox = new QMessageBox(window);
  mbox->addButton(QMessageBox::Ok);
  mbox->setText("Interrupted!");
  mbox->setIcon(QMessageBox::Information);
  QObject::connect(s3, SIGNAL(entered()), mbox, SLOT(exec()));
  s3->addTransition(s1h);
  machine->addState(s3);

  s1->addTransition(interruptButton, SIGNAL(clicked()), s3);

  auto s2 = new QState();
  s1->addTransition(quitButton, SIGNAL(clicked()), s2);
  QObject::connect(s2, &QState::entered, &application, &QApplication::quit);
  machine->addState(s2);

  window->show();
  machine->start();
  application.exec();
}

TEST_CASE_METHOD(TestFixture, "custom_event", "[StateMachine]") {
  auto window = new QFrame;
  auto layout = new QVBoxLayout(window);
  auto btnLayout = new QHBoxLayout;
  auto helloBtn = new QPushButton("Hello");
  auto worldBtn = new QPushButton("world");
  btnLayout->addWidget(helloBtn);
  btnLayout->addWidget(worldBtn);
  layout->addLayout(btnLayout);

  auto msgLabel = new QLabel();
  layout->addWidget(msgLabel);

  QAbstractState *current = nullptr;

  QPointer<QStateMachine> machine = new QStateMachine;
  auto s1 = new QState();
  auto s2 = new QState();
  auto done = new QFinalState;

  auto t1 = new StringTransition("Hello");
  t1->setTargetState(s2);
  s1->addTransition(t1);

  auto t2 = new StringTransition("world");
  t2->setTargetState(done);
  s2->addTransition(t2);
  QObject::connect(s2, &QState::entered, [&]() mutable {
    qDebug() << "s2";
  });

  QObject::connect(done, &QFinalState::entered, [&]()mutable {
    qDebug() << "done";
  });

  machine->addState(s1);
  machine->addState(s2);
  machine->addState(done);
  machine->setInitialState(s1);

  machine->start();

  QObject::connect(helloBtn, &QPushButton::clicked, [&]()mutable {
    machine->postEvent(new StringEvent("Hello"));
  });
  QObject::connect(worldBtn, &QPushButton::clicked, [&]() mutable {
    machine->postEvent(new StringEvent("world"));
  });

  window->show();
  application.exec();
}