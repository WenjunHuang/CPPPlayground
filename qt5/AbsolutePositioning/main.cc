//
// Created by xxzyjy on 2019-03-13.
//

#include <QtWidgets>
#include <QWidget>

int main(int argc, char *argv[]) {
  Q_INIT_RESOURCE(resource);
  QApplication app(argc, argv);

  QWidget window;
  window.resize(600, 400);
  window.setStyleSheet("background-color:#222222");

  auto label1 = new QLabel(&window);
  label1->setPixmap(QPixmap(":/images/icons8-master.png"));
  label1->setGeometry(20, 20, 120, 120);

  auto label2 = new QLabel(&window);
  label2->setPixmap(QPixmap(":/images/icons8-call_in_bed.png"));
  qDebug() << label2->sizeHint();
  qDebug() << label2->minimumSizeHint();

  label2->setGeometry(20, 150, 32, 32);

  window.show();

  return QApplication::exec();
}