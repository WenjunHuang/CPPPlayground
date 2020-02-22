//
// Created by rick on 2020/2/19.
//
#include <QtCore/QtCore>
#include <QtGui/QOpenGLFunctions_3_3_Core>
#include <QtGui/QtGui>

class CameraMouseZoomWindow : public QOpenGLWindow,
                              private QOpenGLFunctions_3_3_Core {
    Q_OBJECT

  public:
    ~CameraMouseZoomWindow(){
        makeCurrent();
    }

  protected:
    void initializeGL() override {
        _timer.start();

        initializeOpenGLFunctions();
        glEnable(GL_DEPTH_TEST);

        // compile shaders
        _shaderProgram = new QOpenGLShaderProgram(this);
        _shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                ":/vertex.glsl");
        _shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                ":/fragment.glsl");
        if (!_shaderProgram->link()) {
            qDebug() << _shaderProgram->log();
        }

        // load textures
        _containerTexture =
            std::make_unique<QOpenGLTexture>(QImage(":/container.jpg"));
        _containerTexture->setWrapMode(QOpenGLTexture::Repeat);
        _containerTexture->setMinMagFilters(QOpenGLTexture::Linear,
                                            QOpenGLTexture::Linear);
        _containerTexture->generateMipMaps();

        _faceTexture =
            std::make_unique<QOpenGLTexture>(QImage(":/awesomeface.png"));
        _faceTexture->setWrapMode(QOpenGLTexture::Repeat);
        _faceTexture->setMinMagFilters(QOpenGLTexture::Linear,
                                       QOpenGLTexture::Linear);
        _faceTexture->generateMipMaps();

        _shaderProgram->bind();
        _shaderProgram->setUniformValue("texture1", 0);
        _shaderProgram->setUniformValue("texture2", 1);

        // load buffers
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

            -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
            0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

        _vao = new QOpenGLVertexArrayObject(this);
        _vao->create();
        _vao->bind();

        _vbo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
        _vbo->create();
        _vbo->bind();
        _vbo->allocate(vertices, sizeof(vertices));

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void paintGL() override {
        auto currentFrame = _timer.elapsed();
        _deltaTime        = currentFrame - _lastFrame;
        _lastFrame        = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        _containerTexture->bind();
        glActiveTexture(GL_TEXTURE1);
        _faceTexture->bind();

        // activate shader
        _shaderProgram->bind();

        QMatrix4x4 projection;
        projection.setToIdentity();
        projection.perspective(_fov, (float)width() / (float)height(), 0.1f,
                               100.0f);
        _shaderProgram->setUniformValue("projection", projection);

        // camera/view transformation
        QMatrix4x4 view;
        view.setToIdentity();
        view.lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
        _shaderProgram->setUniformValue("view", view);

        _vao->bind();
        // world space positions of our cubes
        QVector3D cubePositions[] = {
            QVector3D(0.0f, 0.0f, 0.0f),    QVector3D(2.0f, 5.0f, -15.0f),
            QVector3D(-1.5f, -2.2f, -2.5f), QVector3D(-3.8f, -2.0f, -12.3f),
            QVector3D(2.4f, -0.4f, -3.5f),  QVector3D(-1.7f, 3.0f, -7.5f),
            QVector3D(1.3f, -2.0f, -2.5f),  QVector3D(1.5f, 2.0f, -2.5f),
            QVector3D(1.5f, 0.2f, -1.5f),   QVector3D(-1.3f, 1.0f, -1.5f)};

        for (auto i = 0; i < 10; i++) {
            QMatrix4x4 model;
            model.setToIdentity();
            model.translate(cubePositions[i]);
            float angle = 20.0f * i;
            model.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));
            _shaderProgram->setUniformValue("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    bool event(QEvent* event) override {
        if (event->type() == QEvent::KeyPress) {
            auto keyEvent = dynamic_cast<QKeyEvent*>(event);
            //            auto cameraSpeed = 2.5 * _deltaTime;
            auto cameraSpeed = 2.5f;
            qDebug() << cameraSpeed;
            switch (keyEvent->key()) {
            case Qt::Key_Up:
                _cameraPos += cameraSpeed * _cameraFront;
                requestUpdate();
                break;
            case Qt::Key_Down:
                _cameraPos -= cameraSpeed * _cameraFront;
                requestUpdate();
                break;
            case Qt::Key_Left:
                _cameraPos -= QVector3D::crossProduct(_cameraFront, _cameraUp)
                                  .normalized() *
                              cameraSpeed;
                requestUpdate();
                break;
            case Qt::Key_Right:
                _cameraPos += QVector3D::crossProduct(_cameraFront, _cameraUp)
                                  .normalized() *
                              cameraSpeed;
                requestUpdate();
                break;
            default: break;
            }
        }

        if (event->type() == QEvent::MouseButtonPress) {
            auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
            _lastX          = mouseEvent->x();
            _lastY          = mouseEvent->y();
            mouseEvent->accept();
            return true;
        }

        if (event->type() == QEvent::MouseMove) {
            auto mouseMoveEvent = dynamic_cast<QMouseEvent*>(event);
            if (mouseMoveEvent->buttons() & Qt::LeftButton) {
                float xoffset = mouseMoveEvent->x() - _lastX;
                float yoffset = mouseMoveEvent->y() - _lastY;
                _lastX        = mouseMoveEvent->x();
                _lastY        = mouseMoveEvent->y();

                auto sensitivity = 0.1f;
                xoffset *= sensitivity;
                yoffset *= sensitivity;

                _yaw += xoffset;
                _pitch += yoffset;

                if (_pitch > 89.0f)
                    _pitch = 89.0f;

                if (_pitch < -89.0f)
                    _pitch = -89.9f;

                QVector3D front{std::cos(_yaw) * std::cos(_pitch),
                                std::sin(_pitch),
                                std::sin(_yaw) * std::cos(_pitch)};
                _cameraFront = front.normalized();

                mouseMoveEvent->accept();
                requestUpdate();
                return true;
            }
        }

        return QOpenGLWindow::event(event);
    }

  private:
    QOpenGLShaderProgram* _shaderProgram;
    QOpenGLVertexArrayObject* _vao;
    std::unique_ptr<QOpenGLBuffer> _vbo;
    std::unique_ptr<QOpenGLTexture> _containerTexture;
    std::unique_ptr<QOpenGLTexture> _faceTexture;

    QVector3D _cameraPos{0.0f, 0.0f, 3.0f};
    QVector3D _cameraFront{0.0f, 0.0f, -1.0f};
    QVector3D _cameraUp{0.0f, 1.0f, 0.0f};

    QElapsedTimer _timer;

    float _yaw   = -90.0f;
    float _pitch = 0.0f;
    float _lastX;
    float _lastY;
    float _fov = 45.0f;

    // timing
    qint64 _deltaTime = 0;
    qint64 _lastFrame = 0;
};
int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QSurfaceFormat format;
    format.setSamples(16);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    CameraMouseZoomWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    return app.exec();
}

#include "main.moc"