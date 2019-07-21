#include <QApplication>
#include <QPushButton>

int main(int argc,char *argv[]) {
  QApplication app(argc,argv);
  QPushButton button;
  button.setText("Click Me");
  button.show();

  return QApplication::exec();
}