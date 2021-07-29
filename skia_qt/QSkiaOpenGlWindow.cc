//
// Created by rick on 2021/7/29.
//

#include "QSkiaOpenGlWindow.h"
#include <QDebug>
#include <QOpenGLFunctions>
#include <QTime>
#include <QTimer>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkImage.h>
#include <skia/core/SkSurface.h>
#include <skia/gpu/GrContext.h>
#include <skia/gpu/gl/GrGLInterface.h>

struct QSkiaOpenGLWindowPrivate {
    QOpenGLFunctions funcs;
    sk_sp<GrContext> context = nullptr;
    sk_sp<SkSurface> gpuSurface = nullptr;
    QTimer timer;
    QTime lastTime;
};

QSkiaOpenGLWindow::QSkiaOpenGLWindow(QWindow* parent)
    : QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate, parent)
    , _dptr { new QSkiaOpenGLWindowPrivate }
{
    connect(&_dptr->timer, &QTimer::timeout, this, qOverload<>(&QSkiaOpenGLWindow::update));
    _dptr->timer.start(1000 / 60);
}

void QSkiaOpenGLWindow::initializeGL()
{
    _dptr->funcs.initializeOpenGLFunctions();
    _dptr->context = GrContext::MakeGL();
    SkASSERT(_dptr->context);

    init(width(), height());
    _dptr->lastTime = QTime::currentTime();
}

void QSkiaOpenGLWindow::resizeGL(int w, int h)
{
    init(w, h);
}
void QSkiaOpenGLWindow::paintGL()
{
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

void QSkiaOpenGLWindow::init(int width, int height)
{
    qDebug() << __FUNCTION__ << width << height;
    updateDimensions();
    initSkia();
    _dptr->funcs.glViewport(0, 0, width, height);
}

QSkiaOpenGLWindow::~QSkiaOpenGLWindow()
{
    makeCurrent();
    delete _dptr;
    _dptr = nullptr;
    doneCurrent();
}
void QSkiaOpenGLWindow::updateDimensions()
{
    auto w = width();
    auto h = height();
    qDebug() << QString("Size %1 x %2").arg(w).arg(h);
}
void QSkiaOpenGLWindow::initSkia()
{
    GrBackendRenderTarget renderTarget(width(),
        height(), 0, 8, {
                            0, 0x8058 // GR_GL_RGBA8
                        });
    SkSurfaceProps props(0, SkPixelGeometry::kRGB_H_SkPixelGeometry);
//    _dptr->info = SkImageInfo::MakeN32Premul(width(), height());
    //    _dptr->gpuSurface = SkSurface::MakeRenderTarget(_dptr->context.get(),
    //        SkBudgeted::kNo,
    //        _dptr->info);
    _dptr->gpuSurface = SkSurface::MakeFromBackendRenderTarget(_dptr->context.get(),
        renderTarget, GrSurfaceOrigin::kBottomLeft_GrSurfaceOrigin, SkColorType::kRGBA_8888_SkColorType,
        SkColorSpace::MakeRGB(SkNamedTransferFn::kSRGB, SkNamedGamut::kDisplayP3),
        &props);
    if (!_dptr->gpuSurface) {
        qDebug() << "SkSurface::MakeRenderTarget return null";
        return;
    }
}
void QSkiaOpenGLWindow::draw(SkCanvas* canvas)
{
    canvas->clear(0xffffffff);
    //    canvas->drawRect(SkRect::MakeXYWH(0,0,width(),height()),SkPaint::)
}
