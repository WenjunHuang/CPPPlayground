//
// Created by xxzyjy on 18/12/2017.
//

#include <QWheelEvent>
#include "View.h"
#include "GraphicsView.h"

void GraphicsView::wheelEvent(QWheelEvent *event) {
  if (event->modifiers() & Qt::ControlModifier){
    if (event->delta() > 0)
      view->zoomIn(6);
    else
      view->zoomOut(6);
    event->accept();
  } else {
    QGraphicsView::wheelEvent(event);
  }

}
