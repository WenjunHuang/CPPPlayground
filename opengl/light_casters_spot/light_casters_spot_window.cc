//
// Created by rick on 2021/4/22.
//
#include "light_casters_spot_window.h"
#include <range/v3/all.hpp>
#include <vector>

using namespace ranges;
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};
// positions all containers
QVector<QVector3D> cubePositions = {
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
LightCastersSpotWindow::LightCastersSpotWindow()
{
    setSurfaceType(SurfaceType::OpenGLSurface);

    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setSamples(16);
    setFormat(format);

    resize(800, 600);
    setTitle("light casters spot");
}

void LightCastersSpotWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    // buffers
    _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbo.create();
    _vbo.bind();
    _vbo.allocate(vertices, sizeof(vertices));


    {
        _cubeVAO = std::make_unique<QOpenGLVertexArrayObject>(this);
        QOpenGLVertexArrayObject::Binder binderGuard { _cubeVAO.get() };
        _vbo.bind();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    {
        _lightCubeVAO = std::make_unique<QOpenGLVertexArrayObject>(this);
        QOpenGLVertexArrayObject::Binder binderGuard { _lightCubeVAO.get() };
        _vbo.bind();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
    }

    // textures
    _diffuseMap.setWrapMode(QOpenGLTexture::Repeat);
    _diffuseMap.setData(QImage(":/container2.png"));
    _diffuseMap.bind(1);

    _specularMap.setWrapMode(QOpenGLTexture::Repeat);
    _specularMap.setData(QImage(":/container2_specular.png"));
    _specularMap.bind(2);

    // shaders
    _lightingShader = std::make_unique<QOpenGLShaderProgram>(this);
    _lightingShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/light_casters_vs.glsl");
    _lightingShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/light_casters_fs.glsl");
    _lightingShader->link();
    _lightingShader->bind();
    _lightingShader->setUniformValue("material.diffuse", 1);
    _lightingShader->setUniformValue("material.specular", 1);

    _lightCubeShader = std::make_unique<QOpenGLShaderProgram>(this);
    _lightCubeShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/light_cube_vs.glsl");
    _lightCubeShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/light_cube_fs.glsl");
    _lightCubeShader->link();
}

void LightCastersSpotWindow::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _lightingShader->bind();

    _lightingShader->setUniformValue("light.position", _camera.Position);
    _lightingShader->setUniformValue("light.direction", _camera.Front);
    _lightingShader->setUniformValue("light.cutOff", (float)cos(toRadian(12.5f)));
    _lightingShader->setUniformValue("light.outerCutOff", (float)cos(toRadian(17.5f)));
    _lightingShader->setUniformValue("viewPos", _camera.Position);

    // light properties
    _lightingShader->setUniformValue("light.ambient", QVector3D { 0.1f, 0.1f, 0.1f });
    _lightingShader->setUniformValue("light.diffuse", QVector3D { 0.8f, 0.8f, 0.8f });
    _lightingShader->setUniformValue("light.specular", QVector3D { 1.0f, 1.0f, 1.0f });
    _lightingShader->setUniformValue("light.constant", 1.0f);
    _lightingShader->setUniformValue("light.linear", 0.09f);
    _lightingShader->setUniformValue("light.quadratic", 0.032f);

    // material properties
    _lightingShader->setUniformValue("material.shininess", 32.0f);

    // view/projection transformations
    QMatrix4x4 projection;
    QSizeF s { size() };
    projection.perspective(_camera.Zoom, s.width() / s.height(), 0.1f, 100.0f);
    auto view = _camera.getViewMatrix();
    _lightingShader->setUniformValue("projection", projection);
    _lightingShader->setUniformValue("view", view);

    // world transformation
    QMatrix4x4 model;
    _lightingShader->setUniformValue("model", model);

    // render containers
    {
        QOpenGLVertexArrayObject::Binder binder { _cubeVAO.get() };

        for (const auto& [index, value] : cubePositions | views::enumerate) {
            QMatrix4x4 model;
            model.translate(value);
            float angle = 20.0f * index;
            model.rotate(angle, { 1.0f, 0.3f, 0.5f });
            _lightingShader->setUniformValue("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
}
bool LightCastersSpotWindow::event(QEvent* event)
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
LightCastersSpotWindow::~LightCastersSpotWindow()
{
    makeCurrent();
}
