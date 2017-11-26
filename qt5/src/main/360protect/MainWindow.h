#pragma once


#include <QMainWindow>
#include <QStackedWidget>

class MainWindow : public QMainWindow {
Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);

  ~MainWindow();

protected:
//  bool eventFilter(QObject *, QEvent *) override;
//
//  void mousePressEvent(QMouseEvent *event) override;
//
//  void mouseReleaseEvent(QMouseEvent *event) override;
//
//  void mouseMoveEvent(QMouseEvent *event) override;
//
//  void paintEvent(QPaintEvent *) override;

public slots:
private:
  QStackedWidget *_stackwid;
};



