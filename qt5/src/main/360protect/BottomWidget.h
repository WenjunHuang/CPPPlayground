#pragma once


#include <QWidget>

namespace Ui{
  class BottomWidget;
}

class BottomWidget : public QWidget {
  Q_OBJECT
  Q_PROPERTY(bool fillLowBar
             READ
             fillLowBar
             WRITE
             setFillLowBar
             NOTIFY
             fillLowBarChanged)

public:
  BottomWidget(QWidget *parent = nullptr);
  ~BottomWidget();

  bool fillLowBar() const { return _fillLowBar; }

  void setFillLowBar(bool fillLowBar) {
    _fillLowBar = fillLowBar;
    emit fillLowBarChanged();
  }

  signals:

  void

  fillLowBarChanged();

protected:
  void paintEvent(QPaintEvent *) override;

private:
  bool _fillLowBar;
  QScopedPointer<Ui::BottomWidget> _ui;
};



