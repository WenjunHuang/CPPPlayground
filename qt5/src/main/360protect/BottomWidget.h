#pragma once


#include <QWidget>

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
};



