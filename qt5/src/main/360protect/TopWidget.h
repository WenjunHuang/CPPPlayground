#pragma once
#include <QWidget>

namespace Ui{
  class TopWidget;
}

class TopWidget : public QWidget{
  Q_OBJECT
public:
  TopWidget(QWidget *parent = nullptr);
  ~TopWidget();

signals:
  void onReturn();

protected:
  void paintEvent(QPaintEvent *) override;

private:
  QScopedPointer<Ui::TopWidget> _ui;

};



