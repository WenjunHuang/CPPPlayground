//
// Created by xxzyjy on 18/12/2017.
//

#include <QStyle>
#include <QToolButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>
#include <QGLFormat>
#include <QButtonGroup>
#include <QScrollBar>
#include <qmath.h>
#include "View.h"
#include "GraphicsView.h"

View::View(const QString &name, QWidget *parent) {
  setFrameStyle(Sunken | StyledPanel);
  graphicsView = new GraphicsView(this);
  graphicsView->setRenderHint(QPainter::Antialiasing, false);
  graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
  graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
  graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  auto size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
  QSize iconSize(size, size);

  auto zoomInIcon = new QToolButton;
  zoomInIcon->setAutoRepeat(true);
  zoomInIcon->setAutoRepeatInterval(33);
  zoomInIcon->setAutoRepeatDelay(0);
  zoomInIcon->setIcon(QPixmap(":/images/zoomin.png"));
  zoomInIcon->setIconSize(iconSize);

  auto zoomOutIcon = new QToolButton;
  zoomOutIcon->setAutoRepeat(true);
  zoomOutIcon->setAutoRepeatInterval(33);
  zoomOutIcon->setAutoRepeatDelay(0);
  zoomOutIcon->setIcon(QPixmap(":/images/zoomout.png"));
  zoomOutIcon->setIconSize(iconSize);

  zoomSlider = new QSlider;
  zoomSlider->setMinimum(0);
  zoomSlider->setMaximum(500);
  zoomSlider->setValue(250);
  zoomSlider->setTickPosition(QSlider::TicksRight);

  auto zoomSliderLayout = new QVBoxLayout;
  zoomSliderLayout->addWidget(zoomInIcon);
  zoomSliderLayout->addWidget(zoomSlider);
  zoomSliderLayout->addWidget(zoomOutIcon);

  auto rotateLeftIcon = new QToolButton;
  rotateLeftIcon->setIcon(QPixmap(":/images/rotateleft.png"));
  rotateLeftIcon->setIconSize(iconSize);
  auto rotateRightIcon = new QToolButton;
  rotateRightIcon->setIcon(QPixmap(":/images/rotateright.png"));
  rotateRightIcon->setIconSize(iconSize);

  rotateSlider = new QSlider;
  rotateSlider->setOrientation(Qt::Horizontal);
  rotateSlider->setMinimum(-360);
  rotateSlider->setMaximum(360);
  rotateSlider->setValue(0);
  rotateSlider->setTickPosition(QSlider::TicksBelow);

  auto rotateSliderLayout = new QHBoxLayout;
  rotateSliderLayout->addWidget(rotateLeftIcon);
  rotateSliderLayout->addWidget(rotateSlider);
  rotateSliderLayout->addWidget(rotateRightIcon);

  resetButton = new QToolButton;
  resetButton->setText(tr("0"));
  resetButton->setEnabled(false);

  auto labelLayout = new QHBoxLayout;
  label = new QLabel(name);
  label2 = new QLabel(tr("Pointer Mode"));
  selectModeButton = new QToolButton;
  selectModeButton->setText(tr("Select"));
  selectModeButton->setCheckable(true);
  selectModeButton->setChecked(true);

  dragModeButton = new QToolButton;
  dragModeButton->setText(tr("Drag"));
  dragModeButton->setCheckable(true);
  dragModeButton->setChecked(false);

  antialiasButton = new QToolButton;
  antialiasButton->setText(tr("Antialiasing"));
  antialiasButton->setCheckable(true);
  antialiasButton->setChecked(false);

  openGlButton = new QToolButton;
  openGlButton->setText(tr("OpenGL"));
  openGlButton->setCheckable(true);
  openGlButton->setEnabled(QGLFormat::hasOpenGL());

  printButton = new QToolButton;
  printButton->setIcon(QIcon(QPixmap(":/images/fileprint.png")));

  auto pointerModeGroup = new QButtonGroup(this);
  pointerModeGroup->setExclusive(true);
  pointerModeGroup->addButton(selectModeButton);
  pointerModeGroup->addButton(dragModeButton);

  labelLayout->addWidget(label);
  labelLayout->addStretch();
  labelLayout->addWidget(label2);
  labelLayout->addWidget(selectModeButton);
  labelLayout->addWidget(dragModeButton);
  labelLayout->addStretch();
  labelLayout->addWidget(antialiasButton);
  labelLayout->addWidget(openGlButton);
  labelLayout->addWidget(printButton);

  auto topLayout = new QGridLayout;
  topLayout->addLayout(labelLayout, 0, 0);
  topLayout->addWidget(graphicsView, 1, 0);
  topLayout->addLayout(zoomSliderLayout, 1, 1);
  topLayout->addLayout(rotateSliderLayout, 2, 0);
  topLayout->addWidget(resetButton, 2, 1);
  setLayout(topLayout);

  connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
  connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
  connect(rotateSlider, SIGNAL(valueChanged(int)), this, SLOT(setupmatrix()));
  connect(graphicsView->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(setResetButtonEnabled()));
  connect(graphicsView->horizontalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(setResetButtonEnabled()));
  connect(selectModeButton,SIGNAL(toggled(bool)),this,SLOT(togglePointerMode()));

}

void View::zoomIn(int level) {

}

void View::zoomOut(int level) {

}

void View::resetView() {

}

QGraphicsView *View::view() const {
  return static_cast<QGraphicsView *>(graphicsView);
}

void View::setupMatrix() {
  auto scale = qPow(qreal(2),
                    (zoomSlider->value() - 250) / qreal(50));
  QMatrix matrix;
  matrix.scale(scale,scale);
  matrix.rotate(rotateSlider->value());

  graphicsView->setMatrix(matrix);
  setResetButtonEnabled();

}

void View::setResetButtonEnabled() {
  resetButton->setEnabled(true);

}
