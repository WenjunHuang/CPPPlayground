//
// Created by xxzyjy on 18/12/2017.
//

#include <QSplitter>
#include <QHBoxLayout>
#include "ChipMainWindow.h"
#include "View.h"
#include "Chip.h"

ChipMainWindow::ChipMainWindow(QWidget *parent) : QWidget(parent) {
  populateScene();

  h1Splitter = new QSplitter;
  h2Splitter = new QSplitter;

  auto vSplitter = new QSplitter;
  vSplitter->setOrientation(Qt::Vertical);
  vSplitter->addWidget(h1Splitter);
  vSplitter->addWidget(h2Splitter);

  auto view = new View("Top left view");
  view->view()->setScene(scene);
  h1Splitter->addWidget(view);

  view = new View("Top right view");
  view->view()->setScene(scene);
  h1Splitter->addWidget(view);

  view = new View("Bottom left view");
  view->view()->setScene(scene);
  h2Splitter->addWidget(view);

  view = new View("Bottom right view");
  view->view()->setScene(scene);
  h2Splitter->addWidget(view);

  auto layout = new QHBoxLayout;
  layout->addWidget(vSplitter);
  setLayout(layout);

  setWindowTitle(tr("Chip Example"));

}

void ChipMainWindow::populateScene() {
  scene = new QGraphicsScene(this);
  QImage image(":/images/qt4logo.png");

  int xx = 0;
  int nitems = 0;
  for (int i = -11000; i < 11000; i += 110) {
    ++xx;
    int yy = 0;
    for (int j = -7000; j < 7000; j += 70) {
      ++yy;
      qreal x = (i + 11000) / 22000.0;
      qreal y = (j + 7000) / 14000.0;

      QColor color(image.pixel(int(image.width() * x),
                               int(image.height() * y)));
      QGraphicsItem *item = new Chip(color,xx,yy);
      item->setPos(QPointF(i,j));
      scene->addItem(item);
      ++nitems;
    }
  }
}
