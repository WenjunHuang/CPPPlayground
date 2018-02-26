#pragma once

#include <QFrame>
#include <QGraphicsView>

class GraphicsView;
class QSlider;
class QToolButton;
class QLabel;
class QToolButton;

class View : public QFrame {
Q_OBJECT
public:
  explicit View(const QString &name, QWidget *parent = nullptr);

  QGraphicsView *view() const;

public slots:

  void zoomIn(int level = 1);

  void zoomOut(int level = 1);

private slots:
  void resetView();
  void setupMatrix();

private:
  GraphicsView *graphicsView;

  QSlider *zoomSlider;
  QSlider *rotateSlider;
  QToolButton *resetButton;
  QLabel *label;
  QLabel *label2;
  QToolButton *selectModeButton;
  QToolButton *dragModeButton;
  QToolButton *antialiasButton;
  QToolButton *openGlButton;
  QToolButton *printButton;

  void setResetButtonEnabled();
};



