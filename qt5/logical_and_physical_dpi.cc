//
// Created by rick on 2021/9/27.
//
#include <QApplication>
#include <QFormLayout>
#include <QLabel>
#include <QWidget>
#include <QScreen>

class MainWindow : public QWidget {
  Q_OBJECT
 public:
  MainWindow(QWidget* parent = nullptr) : QWidget(parent) {
    QFormLayout* layout = new QFormLayout();

    QLabel* lb_physical_x = new QLabel();
    lb_physical_x->setText(QString("%1").arg(physicalDpiX()));
    layout->addRow(tr("Physical DPI X:"), lb_physical_x);

    QLabel* lb_physical_y = new QLabel();
    lb_physical_y->setText(QString("%1").arg(physicalDpiY()));
    layout->addRow(tr("Physical DPI Y:"), lb_physical_y);

    QLabel* lb_logical_x = new QLabel();
    lb_logical_x->setText(QString("%1").arg(logicalDpiX()));
    layout->addRow(tr("Logical DPI X:"), lb_logical_x);
    setLayout(layout);

    QLabel* lb_logical_y = new QLabel();
    lb_logical_y->setText(QString("%1").arg(logicalDpiY()));
    layout->addRow(tr("Logical DPI Y:"), lb_logical_y);
    setLayout(layout);

    connect(screen(),&QScreen::logicalDotsPerInchChanged,[=](){
      lb_logical_x->setText(QString("%1").arg(logicalDpiX()));
      lb_logical_y->setText(QString("%1").arg(logicalDpiY()));
    });
  }
};

int main(int argc, char* argv[]) {
//  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication app(argc, argv);
  MainWindow window;
  window.show();

  return app.exec();
}

#include "logical_and_physical_dpi.moc"