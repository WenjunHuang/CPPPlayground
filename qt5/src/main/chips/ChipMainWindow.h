#pragma once

#include <QWidget>

class QSplitter;
class QGraphicsScene;

class ChipMainWindow : public QWidget{
  Q_OBJECT
public:
  ChipMainWindow(QWidget *parent = nullptr);

private:
  QGraphicsScene *scene;
  QSplitter *h1Splitter;
  QSplitter *h2Splitter;


  void populateScene();
};



