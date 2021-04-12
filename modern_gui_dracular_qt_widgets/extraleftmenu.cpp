#include "extraleftmenu.h"
#include "main_window.h"
#include "ui_extraleftmenu.h"
#include <QPropertyAnimation>
#include <QSignalTransition>
#include <QtWidgets/private/qframe_p.h>

struct ExtraLeftMenuPrivate : QFramePrivate {
    Q_DECLARE_PUBLIC(ExtraLeftMenu)

    Ui::ExtraLeftMenu* ui        = new Ui::ExtraLeftMenu();
    QStateMachine* _stateMachine = new QStateMachine();

    ~ExtraLeftMenuPrivate() final {
        delete ui;
        delete _stateMachine;
    }

    void initialize() {
        Q_Q(ExtraLeftMenu);

        ui->setupUi(q);

        auto collapsed = new QState();
        collapsed->assignProperty(q, "minimumWidth", 10);
        auto expanded      = new QState();
        auto preferredSize = q->sizeHint();
        expanded->assignProperty(q, "minimumWidth", preferredSize.width());

        auto animation1 = new QPropertyAnimation(q, "minimumWidth");
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
};

ExtraLeftMenu::ExtraLeftMenu(QWidget* parent)
    : QFrame(*new ExtraLeftMenuPrivate(), parent) {
    Q_D(ExtraLeftMenu);
    d->initialize();
}

ExtraLeftMenu::~ExtraLeftMenu() {}
