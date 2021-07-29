//
// Created by rick on 2021/7/29.
//

#pragma once
#include <QOpenGLWindow>
#include <memory>

class QSkiaOpenGLWindowPrivate;
class SkCanvas;

class QSkiaOpenGLWindow : public QOpenGLWindow {
    Q_OBJECT
public:
    QSkiaOpenGLWindow(QWindow* parent = nullptr);
    ~QSkiaOpenGLWindow() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void draw(SkCanvas* canvas);

private:
    void init(int width,int height);
    void updateDimensions();
    void initSkia();


private:
    QSkiaOpenGLWindowPrivate *_dptr;
};
