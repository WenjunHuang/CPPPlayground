#ifndef EXTRALEFTMENU_H
#define EXTRALEFTMENU_H

#include <QFrame>
#include <qstatemachine.h>

namespace Ui {
    class ExtraLeftMenu;
}
class ExtraLeftMenuPrivate;

class ExtraLeftMenu : public QFrame {
    Q_OBJECT

  public:
    explicit ExtraLeftMenu(QWidget* parent = nullptr);
    ~ExtraLeftMenu();
  private:
    Q_DECLARE_PRIVATE(ExtraLeftMenu);

};

#endif // EXTRALEFTMENU_H
