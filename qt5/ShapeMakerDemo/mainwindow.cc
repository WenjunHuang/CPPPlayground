#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "shape_canvas.h"

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->shapeCombo->addItem(tr("Polygon"), QVariant::fromValue(ShapeCanvas::Shape::Polygon));
  ui->shapeCombo->addItem(tr("Rectangle"), QVariant::fromValue(ShapeCanvas::Shape::Rect));
  ui->shapeCombo->addItem(tr("Rounded Rectangle"), QVariant::fromValue(ShapeCanvas::Shape::RoundedRect));
  ui->shapeCombo->addItem(tr("Ellipse"), QVariant::fromValue(ShapeCanvas::Shape::Ellipse));
  ui->shapeCombo->addItem(tr("Pie"), QVariant::fromValue(ShapeCanvas::Shape::Pie));
  ui->shapeCombo->addItem(tr("Chord"), QVariant::fromValue(ShapeCanvas::Shape::Chord));
  ui->shapeCombo->addItem(tr("Text"), QVariant::fromValue(ShapeCanvas::Shape::Text));
  ui->shapeCombo->addItem(tr("Pixmap"), QVariant::fromValue(ShapeCanvas::Shape::Pixmap));

  // Pen style
  ui->penStyle->addItem(tr("Solid"), QVariant::fromValue(Qt::SolidLine));
  ui->penStyle->addItem(tr("Dash"), QVariant::fromValue(Qt::DashLine));
  ui->penStyle->addItem(tr("Dot"), QVariant::fromValue(Qt::DotLine));
  ui->penStyle->addItem(tr("Dash Dot"), QVariant::fromValue(Qt::DashDotLine));
  ui->penStyle->addItem(tr("Dash Dot Dot"), QVariant::fromValue(Qt::DashDotDotLine));
  ui->penStyle->addItem(tr("None"), QVariant::fromValue(Qt::NoPen));

  // Pencap combobox
  ui->penCap->addItem(tr("Flat"), QVariant::fromValue(Qt::FlatCap));
  ui->penCap->addItem(tr("Square"), QVariant::fromValue(Qt::SquareCap));
  ui->penCap->addItem(tr("Round"), QVariant::fromValue(Qt::RoundCap));

  // Pen join combobox
  ui->penJoin->addItem(tr("Miter"), QVariant::fromValue(Qt::MiterJoin));
  ui->penJoin->addItem(tr("Bevel"), QVariant::fromValue(Qt::BevelJoin));
  ui->penJoin->addItem(tr("Round"), QVariant::fromValue(Qt::RoundJoin));

  ui->brushStyle->addItem(tr("Linear Gradient"), QVariant::fromValue(Qt::LinearGradientPattern));
  ui->brushStyle->addItem(tr("Radial Gradient"), QVariant::fromValue(Qt::RadialGradientPattern));
  ui->brushStyle->addItem(tr("Conical Gradient"), QVariant::fromValue(Qt::ConicalGradientPattern));
  ui->brushStyle->addItem(tr("None"), QVariant::fromValue(Qt::NoBrush));

  _canvas = new ShapeCanvas{this};
  ui->canvasLayout->addWidget(_canvas);

  penChanged();
  brushChanged();
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_shapeCombo_activated(int index) {
  auto shape = ui->shapeCombo->currentData().value<ShapeCanvas::Shape>();
  _canvas->setShape(shape);
}

void MainWindow::on_penWidth_valueChanged(int value) {
  Q_UNUSED(value);
  penChanged();
}


void MainWindow::on_penStyle_activated(int index) {
  Q_UNUSED(index);
  penChanged();
}


void MainWindow::on_penCap_activated(int index) {
  Q_UNUSED(index);
  penChanged();
}


void MainWindow::on_penJoin_activated(int index) {
  Q_UNUSED(index);
  penChanged();
}


void MainWindow::on_brushStyle_activated(int index) {
  brushChanged();
}


void MainWindow::on_antialising_toggled(bool checked) {
  _canvas->setAntialiased(checked);
}


void MainWindow::on_transforms_toggled(bool checked) {
  _canvas->setTransformed(checked);
}

void MainWindow::penChanged() {
  auto penWidth = ui->penWidth->value();
  auto style = ui->penStyle->itemData(ui->penStyle->currentIndex()).value<Qt::PenStyle>();
  auto cap = ui->penCap->currentData().value<Qt::PenCapStyle>();
  auto join = ui->penJoin->currentData().value<Qt::PenJoinStyle>();
  QPen pen{};
  pen.setWidth(penWidth);
  pen.setStyle(style);
  pen.setCapStyle(cap);
  pen.setJoinStyle(join);
  _canvas->setPen(pen);
}

void MainWindow::brushChanged() {
  auto style = ui->brushStyle->currentData().value<Qt::BrushStyle>();
  switch (style) {
    case Qt::LinearGradientPattern: {
      QLinearGradient linearGradient{0, 0, 100, 100};
      linearGradient.setColorAt(0.0, Qt::red);
      linearGradient.setColorAt(0.2, Qt::green);
      linearGradient.setColorAt(1.0, Qt::blue);
      _canvas->setBrush(linearGradient);
      break;
    }
    case Qt::RadialGradientPattern: {
      QRadialGradient radialGradient{50, 50, 50, 70, 70};
      radialGradient.setColorAt(0.0, Qt::red);
      radialGradient.setColorAt(0.2, Qt::green);
      radialGradient.setColorAt(1.0, Qt::blue);
      _canvas->setBrush(radialGradient);
      break;
    }
    case Qt::ConicalGradientPattern: {
      QConicalGradient conicalGradient{50, 50, 150};
      conicalGradient.setColorAt(0.0, Qt::red);
      conicalGradient.setColorAt(0.2, Qt::green);
      conicalGradient.setColorAt(1.0, Qt::blue);
      _canvas->setBrush(conicalGradient);
      break;
    }
    case Qt::TexturePattern: {
      _canvas->setBrush(QBrush{QPixmap{":/images/qt.png"}});
      break;
    }
    default:
      _canvas->setBrush(QBrush{Qt::blue, style});
  }

}

