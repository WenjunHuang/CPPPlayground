//
// Created by rick on 2021/4/22.
//

#pragma once

#include "camera.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWindow>
#include <QtGui>
#include <memory>

class LightCastersSpotWindow : public QOpenGLWindow, private QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    LightCastersSpotWindow();
    ~LightCastersSpotWindow();

protected:
    void initializeGL() override;
    void paintGL() override;
    bool event(QEvent* event) override;

private:
    std::unique_ptr<QOpenGLVertexArrayObject> _cubeVAO;
    std::unique_ptr<QOpenGLVertexArrayObject> _lightCubeVAO;
    QOpenGLBuffer _vbo { QOpenGLBuffer::VertexBuffer };
    QOpenGLTexture _diffuseMap { QOpenGLTexture::Target2D };
    QOpenGLTexture _specularMap { QOpenGLTexture::Target2D };
    std::unique_ptr<QOpenGLShaderProgram> _lightingShader;
    std::unique_ptr<QOpenGLShaderProgram> _lightCubeShader;
    Camera _camera { QVector3D { 0.0f, 0.0f, 3.0f } };

    int _lastX;
    int _lastY;
};
