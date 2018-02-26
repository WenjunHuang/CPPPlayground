#pragma once


#include <QGraphicsView>

class View;
class GraphicsView : public QGraphicsView{
  Q_OBJECT
public:
  GraphicsView(View *v):QGraphicsView(),view(v){}
protected:
  void wheelEvent(QWheelEvent *) override;
private:
  View *view;

};



