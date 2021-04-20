//
// Created by rick on 2021/4/20.
//

#pragma once
#include "camera.h"
#include <QElapsedTimer>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWindow>
class BasicLightingWindow : public QOpenGLWindow ,private QOpenGLFunctions_4_5_Core{
    Q_OBJECT
public:
    BasicLightingWindow();
protected:
    void initializeGL() override;
    void paintGL() override;
    bool event(QEvent* event) override;

private:
    QOpenGLVertexArrayObject *_vao{nullptr};
    QOpenGLVertexArrayObject *_lightCubeVAO{nullptr};
    QOpenGLShaderProgram * _lightingShader {nullptr};
    QOpenGLShaderProgram *_lightCubeShader{nullptr};
    QOpenGLBuffer _vbo { QOpenGLBuffer::VertexBuffer };
    QOpenGLTexture *_diffuse{nullptr};
    QOpenGLTexture *_specular{nullptr};
    QElapsedTimer _timer;
    Camera _camera{{ 0.0f, 0.0f, 3.0f }};
    int _lastX;
    int _lastY;
    long _lastFrame{0L};

};
