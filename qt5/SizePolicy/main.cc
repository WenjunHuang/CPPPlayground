//
// Created by xxzyjy on 2019-03-06.
//

#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>

int main(int argc,char *argv[]){
  QApplication app(argc,argv);

  QWidget window;
  auto layout = new QHBoxLayout();
  window.setLayout(layout);

  QPushButton button;
  button.setText("Text");
  layout->addWidget(&button);

  QLabel label;
  label.setText("helleo world");
  layout->addWidget(&label);

  layout->setSizeConstraint(QLayout::SetFixedSize);

  window.show();

  return QApplication::exec();
}