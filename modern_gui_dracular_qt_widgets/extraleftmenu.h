#ifndef EXTRALEFTMENU_H
#define EXTRALEFTMENU_H

#include <QFrame>
#include <qstatemachine.h>

namespace Ui {
    class ExtraLeftMenu;
}

class ExtraLeftMenu : public QFrame {
    Q_OBJECT

  public:
    explicit ExtraLeftMenu(QWidget* parent = nullptr);
    ~ExtraLeftMenu();

  private:
    Ui::ExtraLeftMenu* ui;
    QStateMachine *_stateMachine;
};

#endif // EXTRALEFTMENU_H
