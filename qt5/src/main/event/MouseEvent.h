#pragma once


#include <QMainWindow>
#include <QLabel>

class MouseEvent : public QMainWindow {
Q_OBJECT
public:
  MouseEvent(QWidget *parent = nullptr);

  ~MouseEvent();

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
  QLabel *statusLabel;
  QLabel *mousePosLabel;


};



