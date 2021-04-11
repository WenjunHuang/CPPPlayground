#include "extraleftmenu.h"
#include "main_window.h"
#include "ui_extraleftmenu.h"
#include <QPropertyAnimation>
#include <QSignalTransition>

ExtraLeftMenu::ExtraLeftMenu(QWidget* parent)
    : QFrame(parent), ui(new Ui::ExtraLeftMenu) {
    ui->setupUi(this);

    _stateMachine  = new QStateMachine(this);
    auto collapsed = new QState();
    collapsed->assignProperty(this, "minimumWidth", 60);
    auto expanded = new QState();
    auto preferredSize = this->sizeHint();
    expanded->assignProperty(this, "minimumWidth", preferredSize.width());

    auto animation1 = new QPropertyAnimation(this, "minimumWidth");
    animation1->setDuration(500);
    animation1->setEasingCurve(QEasingCurve::InOutQuart);

    auto collapsedToExpandedTransition = collapsed->addTransition(
        ui->toggleButton, &QPushButton::clicked, expanded);
    collapsedToExpandedTransition->addAnimation(animation1);

    auto expandedToCollapsedTransition = expanded->addTransition(
        ui->toggleButton, &QPushButton::clicked, collapsed);
    expandedToCollapsedTransition->addAnimation(animation1);

    _stateMachine->addState(collapsed);
    _stateMachine->addState(expanded);
    _stateMachine->setInitialState(collapsed);
    _stateMachine->start();
}

ExtraLeftMenu::~ExtraLeftMenu() { delete ui; }
