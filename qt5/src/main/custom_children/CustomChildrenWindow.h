#ifndef CustomChildrenWindow_H
#define CustomChildrenWindow_H

#include <QMainWindow>

namespace Ui {
class CustomChildrenWindow;
}

class CustomChildrenWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CustomChildrenWindow(QWidget *parent = 0);
    ~CustomChildrenWindow();

private slots:
  void on_tabWidget_currentChanged(int index);

private:
    Ui::CustomChildrenWindow *ui;
};

#endif // CustomChildrenWindow_H
