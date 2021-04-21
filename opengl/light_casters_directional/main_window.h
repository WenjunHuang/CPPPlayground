//
// Created by rick on 2021/4/21.
//

#pragma once

#include "camera.h"
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWindow>

class LightCastersDirectionalWindow : public QOpenGLWindow, private QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    LightCastersDirectionalWindow();

protected:
    void initializeGL() override;
    void paintGL() override;
    bool event(QEvent* event) override;

private:
    QOpenGLShaderProgram* _lightingShader { nullptr };
    QOpenGLShaderProgram* _lightCubeShader { nullptr };
    QOpenGLVertexArrayObject* _cubeVAO { nullptr };
    QOpenGLVertexArrayObject* _lightCubeVAO{nullptr};

    QOpenGLTexture _containerTexture{QOpenGLTexture::Target2D};
    QOpenGLTexture _specularTexture{QOpenGLTexture::Target2D};
    QOpenGLBuffer _vbo { QOpenGLBuffer::VertexBuffer };

    Camera _camera{QVector3D{0.0f,0.0f,3.0f}};

    int _lastX;
    int _lastY;
};
