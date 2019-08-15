#pragma once


#include <QMainWindow>
#include <QStackedWidget>
#include "MaskMainWidget.h"

class MainWindowPrivate;

class MainWindow : public QMainWindow {
Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);

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
  QScopedPointer<MainWindowPrivate> d_ptr;

  Q_DECLARE_PRIVATE(MainWindow)

  void initializeAnimations();
};



