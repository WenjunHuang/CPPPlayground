//
// Created by xxzyjy on 2019-01-03.
//

#include <QPainter>
#include "OpenGLWindow.h"

OpenGLWindow::OpenGLWindow(QWindow *parent) : QWindow(parent),
                                              _animating{false},
                                              _context{nullptr},
                                              _device{nullptr} {
  setSurfaceType(QWindow::OpenGLSurface);
  QSurfaceFormat sf;
  sf.setSamples(16);
  sf.setVersion(3, 3);
  sf.setProfile(QSurfaceFormat::CoreProfile);
  setFormat(sf);
}

OpenGLWindow::~OpenGLWindow() {
  delete _device;
}

void OpenGLWindow::render(QPainter *painter) {
  Q_UNUSED(painter);
}

void OpenGLWindow::initialize() {}

void OpenGLWindow::render() {
  if (!_device)
    _device = new QOpenGLPaintDevice;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  const qreal retinaScale = devicePixelRatio();
  glViewport(0, 0, width() * retinaScale, height() * retinaScale);

  _device->setSize(size());

  QPainter painter(_device);
  render(&painter);
}

void OpenGLWindow::renderLater() {
  requestUpdate();
}

bool OpenGLWindow::event(QEvent *event) {
  switch (event->type()) {
    case QEvent::UpdateRequest:
      renderNow();
      return true;
    default:
      return QWindow::event(event);
  }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event) {
  Q_UNUSED(event);

  if (isExposed())
    renderNow();
}

void OpenGLWindow::renderNow() {
  if (!isExposed())
    return;

  bool needsInitialize = false;
  if (!_context) {
    _context = new QOpenGLContext(this);
    _context->setFormat(requestedFormat());
    _context->create();

    needsInitialize = true;
  }

  _context->makeCurrent(this);

  if (needsInitialize) {
    initializeOpenGLFunctions();
    initialize();
  }

  render();

  _context->swapBuffers(this);

  if (_animating)
    renderLater();
}

void OpenGLWindow::setAnimating(bool animating) {
  _animating = animating;
  if (animating)
    renderLater();
}