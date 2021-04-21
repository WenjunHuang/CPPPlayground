//
// Created by rick on 2021/4/21.
//
#include <ranges>
#include <vector>
#include <QKeyEvent>
#include "main_window.h"
// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};
// positions all containers
std::vector<QVector3D> cubePositions = {
    QVector3D(0.0f, 0.0f, 0.0f),
    QVector3D(2.0f, 5.0f, -15.0f),
    QVector3D(-1.5f, -2.2f, -2.5f),
    QVector3D(-3.8f, -2.0f, -12.3f),
    QVector3D(2.4f, -0.4f, -3.5f),
    QVector3D(-1.7f, 3.0f, -7.5f),
    QVector3D(1.3f, -2.0f, -2.5f),
    QVector3D(1.5f, 2.0f, -2.5f),
    QVector3D(1.5f, 0.2f, -1.5f),
    QVector3D(-1.3f, 1.0f, -1.5f)
};

void LightCastersDirectionalWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    _vbo.create();
    _vbo.bind();
    _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbo.allocate(vertices, sizeof(vertices));

    {
        _cubeVAO = new QOpenGLVertexArrayObject { this };
        QOpenGLVertexArrayObject::Binder binder { _cubeVAO };
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    {
        _vbo.bind();
        _lightCubeVAO = new QOpenGLVertexArrayObject { this };
        QOpenGLVertexArrayObject::Binder binder { _lightCubeVAO };
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
    }

    // load textures
    _containerTexture.setWrapMode(QOpenGLTexture::Repeat);
    _containerTexture.setData(QImage(":/container2.png"));
    _containerTexture.bind(1);

    _specularTexture.setWrapMode(QOpenGLTexture::Repeat);
    _specularTexture.setData(QImage(":/container2_specular.png"));
    _specularTexture.bind(2);

    // shader
    _lightingShader = new QOpenGLShaderProgram { this };
    _lightingShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/light_casters_vs.glsl");
    _lightingShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/light_casters_fs.glsl");
    _lightingShader->link();
    _lightingShader->bind();

    _lightCubeShader = new QOpenGLShaderProgram { this };
    _lightCubeShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/light_cube_vs.glsl");
    _lightCubeShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/light_cube_fs.glsl");
    _lightCubeShader->link();
}

LightCastersDirectionalWindow::LightCastersDirectionalWindow()
{
    QSurfaceFormat format;
    format.setSamples(16);
    format.setMajorVersion(4);
    format.setMinorVersion(5);
    format.setProfile(QSurfaceFormat::CoreProfile);

    setFormat(format);
    setSurfaceType(SurfaceType::OpenGLSurface);
    resize(600, 400);
}

void LightCastersDirectionalWindow::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _lightingShader->bind();
    _lightingShader->setUniformValue("light.direction", QVector3D { -0.2f, -1.0f, -0.3f });
    _lightingShader->setUniformValue("viewPos", _camera.Position);

    // light properties
    _lightingShader->setUniformValue("light.ambient", QVector3D { 0.2f, 0.2f, 0.2f });
    _lightingShader->setUniformValue("light.diffuse", QVector3D { 0.5f, 0.5f, 0.5f });
    _lightingShader->setUniformValue("light.specular", QVector3D { 1.0f, 1.0f, 1.0f });

    // material properties
    _lightingShader->setUniformValue("material.shininess", 32.0f);

    // view/projection transformations
    QMatrix4x4 projection;
    QSizeF viewSize { size() };
    projection.perspective(_camera.Zoom, viewSize.width()/viewSize.height(),0.1f,100.0f);

    auto view = _camera.getViewMatrix();
    _lightingShader->setUniformValue("projection",projection);
    _lightingShader->setUniformValue("view",view);

    // world transformation
    QMatrix4x4 model;
    _lightingShader->setUniformValue("model",model);

    // bind diffuse map
    _containerTexture.bind(1);
    // bind specular map
    _specularTexture.bind(2);

    _lightingShader->setUniformValue("material.diffuse", 1);
    _lightingShader->setUniformValue("material.specular", 2);

    // render containers;
    _cubeVAO->bind();
    size_t index = 0;
    for (const auto& value : cubePositions ) {
        QMatrix4x4 model;
        model.translate(value);
        auto angle = 20.0f * (index++);

        model.rotate(angle,QVector3D{1.0f,0.3f,0.5f});
        _lightingShader->setUniformValue("model",model);

        glDrawArrays(GL_TRIANGLES,0,36);
    }
}

bool LightCastersDirectionalWindow::event(QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        auto keyEvent = dynamic_cast<QKeyEvent*>(event);
        switch (keyEvent->key()) {
        case Qt::Key_Up:
            _camera.processKeyboard(CameraMovement::FORWARD);
            requestUpdate();
            keyEvent->accept();
            return true;
        case Qt::Key_Down:
            _camera.processKeyboard(CameraMovement::BACKWARD);
            requestUpdate();
            keyEvent->accept();
            return true;
        case Qt::Key_Left:
            _camera.processKeyboard(CameraMovement::LEFT);
            requestUpdate();
            keyEvent->accept();
            return true;
        case Qt::Key_Right:
            _camera.processKeyboard(CameraMovement::RIGHT);
            requestUpdate();
            keyEvent->accept();
            return true;
        }
    }

    if (event->type() == QEvent::MouseButtonPress) {
        auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
        _lastX = mouseEvent->x();
        _lastY = mouseEvent->y();
        mouseEvent->accept();
        return true;
    }

    if (event->type() == QEvent::MouseMove) {
        auto mouseMoveEvent = dynamic_cast<QMouseEvent*>(event);
        if (mouseMoveEvent->buttons() & Qt::LeftButton) {
            float xoffset = mouseMoveEvent->x() - _lastX;
            float yoffset = mouseMoveEvent->y() - _lastY;
            _lastX = mouseMoveEvent->x();
            _lastY = mouseMoveEvent->y();

            _camera.processMouseMovement(xoffset, yoffset);

            mouseMoveEvent->accept();
            requestUpdate();
            return true;
        }
    }

    if (event->type() == QEvent::Wheel) {
        auto wheelEvent = dynamic_cast<QWheelEvent*>(event);
        auto delta = wheelEvent->angleDelta() / 8;
        _camera.processMouseScroll(delta.y());
        wheelEvent->accept();
        requestUpdate();
        return true;
    }
    return QOpenGLWindow::event(event);
}
