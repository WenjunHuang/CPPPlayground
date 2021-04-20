//
// Created by rick on 2021/4/20.
//

#include "basic_lighting_window.h"
#include "camera.h"
#include <QKeyEvent>

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};

BasicLightingWindow::BasicLightingWindow()
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setMajorVersion(4);
    format.setMinorVersion(5);
    format.setSamples(16);

    setSurfaceType(SurfaceType::OpenGLSurface);
    setFormat(format);
}

void BasicLightingWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    // 创建cube的 vao
    _vao = new QOpenGLVertexArrayObject { this };
    _vao->create();
    _vao->bind();

    // 先将vertex 数据上传到gpu
    _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (!_vbo.create())
        qDebug() << "can not create vertex buffer";
    _vbo.bind();
    _vbo.allocate(vertices, sizeof(vertices));

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 灯光 cube的vao
    _lightCubeVAO = new QOpenGLVertexArrayObject{this};
    _lightCubeVAO->create();
    _lightCubeVAO->bind();

    _vbo.bind();
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(float),nullptr);
    glEnableVertexAttribArray(0);

    // shader program
    _lightingShader = new QOpenGLShaderProgram { this };
    _lightingShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/basic_lighting_vs.glsl");
    _lightingShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/basic_lighting_fs.glsl");
    if (!_lightingShader->link()) {
        qDebug() << _lightingShader->log();
    }

    _lightCubeShader = new QOpenGLShaderProgram { this };
    _lightCubeShader->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                              ":/light_cube_vs.glsl");
    _lightCubeShader->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                              ":/light_cube_fs.glsl");

    if (!_lightCubeShader->link()) {
        qDebug() << _lightCubeShader->log();
    }

    _timer.start();

}

void BasicLightingWindow::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    QVector3D lightPos { 1.2f, 1.0f, 2.0f };
//    QVector3D lightPos { 0.0f, 0.0f, 2.0f };

    _lightingShader->bind();
    _lightingShader->setUniformValue("objectColor", QVector3D { 1.0f, 0.5f, 0.31f });
    _lightingShader->setUniformValue("lightColor", QVector3D { 1.0f, 1.0f, 1.0f });
    _lightingShader->setUniformValue("lightPos", lightPos);

    // view/projection transformations
    auto s = size();
    QMatrix4x4 projection;
    projection.perspective(_camera.Zoom, (float)s.width() / (float)s.height(), 0.1f, 100.0f);
    auto view = _camera.getViewMatrix();

    _lightingShader->setUniformValue("projection", projection);
    _lightingShader->setUniformValue("view", view);
    _lightingShader->setUniformValue("viewPos",_camera.Position);

    // world transformation
    QMatrix4x4 model;
    _lightingShader->setUniformValue("model", model);

    // render the cube
    _vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw the lamp object
    _lightCubeShader->bind();
    _lightCubeShader->setUniformValue("projection",projection);
    _lightCubeShader->setUniformValue("view",view);
    QMatrix4x4 cubeModel;
    cubeModel.translate(lightPos);
    cubeModel.scale(0.2f);
    _lightCubeShader->setUniformValue("model",cubeModel);

    _lightCubeVAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // also draw the lamp object
    context()->swapBuffers(this);
    makeCurrent();
}
bool BasicLightingWindow::event(QEvent* event)
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

    if (event->type()== QEvent::Wheel) {
        auto wheelEvent = dynamic_cast<QWheelEvent*>(event);
        auto delta = wheelEvent->angleDelta() / 8;
        _camera.processMouseScroll(delta.y());
        wheelEvent->accept();
        requestUpdate();
        return true;
    }
    return QOpenGLWindow::event(event);
}