//
// Created by rick on 2021/7/29.
//

#include "QSkiaOpenGlWindow.h"
#include <QtCore>
#include <QtGui>
#include <memory>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkImage.h>
#include <skia/core/SkSurface.h>
#include <skia/effects/SkGradientShader.h>
#include <skia/gpu/GrDirectContext.h>
#include <skia/gpu/gl/GrGLInterface.h>
#include "Scenes.h"

struct QSkiaOpenGLWindowPrivate {
    QOpenGLFunctions        funcs;
    sk_sp<GrDirectContext>        context    = nullptr;
    sk_sp<SkSurface>        gpuSurface = nullptr;
    QTimer                  timer;
    QTime                   lastTime;
    std::unique_ptr<Scenes> scenes = std::make_unique<Scenes>();
    QPoint                  mousePos;
};

QSkiaOpenGLWindow::QSkiaOpenGLWindow(float scale, QWindow* parent)
    : QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate, parent),
      _scale{scale}, _dptr{new QSkiaOpenGLWindowPrivate} {
    connect(&_dptr->timer,
            &QTimer::timeout,
            this,
            qOverload<>(&QSkiaOpenGLWindow::update));
    _dptr->timer.start(1000 / 60);
}

void QSkiaOpenGLWindow::initializeGL() {
    _dptr->funcs.initializeOpenGLFunctions();
    _dptr->context = GrDirectContext::MakeGL();
    SkASSERT(_dptr->context);

    init(width(), height());
    _dptr->lastTime = QTime::currentTime();
}

void QSkiaOpenGLWindow::resizeGL(int w, int h) {
    updateDimensions();
    initSkia();
}

void QSkiaOpenGLWindow::paintGL() {
    if (!isVisible())
        return;
    if (!_dptr->gpuSurface)
        return;

    auto canvas = _dptr->gpuSurface->getCanvas();
    canvas->save();
    draw(canvas);
    canvas->flush();
    canvas->restore();
}

void QSkiaOpenGLWindow::init(int width, int height) {
    updateDimensions();
    initSkia();
    _dptr->funcs.glViewport(0, 0, width, height);
}

QSkiaOpenGLWindow::~QSkiaOpenGLWindow() {
    makeCurrent();
    delete _dptr;
    _dptr = nullptr;
    doneCurrent();
}

void QSkiaOpenGLWindow::updateDimensions() {
    auto w = width();
    auto h = height();
}

void QSkiaOpenGLWindow::initSkia() {
    GrBackendRenderTarget renderTarget(width(),
                                       height(),
                                       0,
                                       8,
                                       {
                                           0, 0x8058 // GR_GL_RGBA8
                                       });
    SkSurfaceProps        props(0, SkPixelGeometry::kRGB_H_SkPixelGeometry);

    _dptr->gpuSurface = nullptr;
    _dptr->gpuSurface = SkSurface::MakeFromBackendRenderTarget(
        _dptr->context.get(),
        renderTarget,
        GrSurfaceOrigin::kBottomLeft_GrSurfaceOrigin,
        SkColorType::kRGBA_8888_SkColorType,
        SkColorSpace::MakeRGB(SkNamedTransferFn::kSRGB,
                              SkNamedGamut::kDisplayP3),
        &props);
    if (!_dptr->gpuSurface) {
        qDebug() << "SkSurface::MakeRenderTarget return null";
        return;
    }
}

void QSkiaOpenGLWindow::draw(SkCanvas* canvas) {
    _dptr->scenes->draw(canvas,
                        width(),
                        height(),
                        _scale,
                        _dptr->mousePos.x(),
                        _dptr->mousePos.y());
}

void QSkiaOpenGLWindow::mouseMoveEvent(QMouseEvent* event) {
    _dptr->mousePos = event->pos() / _scale;
}