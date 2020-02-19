//
// Created by HUANG WEN JUN on 2020/2/19.
//
#include <QtCore/QtCore>
#include <QtGui/QOpenGLFunctions_3_3_Core>
#include <QtGui/QtGui>

class MyWindow : public QWindow, private QOpenGLFunctions_3_3_Core {
    Q_OBJECT
  public:
    MyWindow() {
        setSurfaceType(SurfaceType::OpenGLSurface);
        _timer.start();
        _repaintTimer = new QTimer(this);
        _repaintTimer->setInterval(1000);
        connect(_repaintTimer,&QTimer::timeout,this,QOverload<>::of(&MyWindow::requestUpdate));
    }

  protected:
    bool event(QEvent* event) override {
        switch (event->type()) {
        case QEvent::UpdateRequest: renderNow(); return true;
        case QEvent::KeyPress:{
            float cameraSpeed=  0.5f;
            auto keyEvent = dynamic_cast<QKeyEvent*>(event);
            switch(keyEvent->key()) {
            case Qt::Key_Up:
                _cameraPos += cameraSpeed * _cameraFront;
                requestUpdate();
                break;
            case Qt::Key_Down:
                _cameraPos -= cameraSpeed * _cameraFront;
                requestUpdate();
                break;
            case Qt::Key_Left:
                _cameraPos -= QVector3D::crossProduct(_cameraFront,_cameraUp).normalized() * cameraSpeed;
                requestUpdate();
                break;
            case Qt::Key_Right:
                _cameraPos += QVector3D::crossProduct(_cameraFront,_cameraUp).normalized() * cameraSpeed;
                requestUpdate();
                break;
            }
            return true;
        }
        default: QWindow::event(event);
        }
    }

    void exposeEvent(QExposeEvent* event) override {
        Q_UNUSED(event);
        if (isExposed())
            renderNow();
    }

  private:
    void renderNow() {
        bool needInit = false;
        if (!_context) {
            _context = new QOpenGLContext(this);
            _context->setFormat(requestedFormat());
            _context->create();
            needInit = true;
        }
        _context->makeCurrent(this);
        if (needInit) {
            initialize();
        }

        const qreal retinaScale = devicePixelRatio();
        glViewport(0, 0, width() * retinaScale, height() * retinaScale);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        _containerTexture->bind();
        glActiveTexture(GL_TEXTURE1);
        _faceTexture->bind();

        // active shader
        _shaderProgram->bind();

        // camera/view transformation
        QMatrix4x4 view;
        view.setToIdentity();
        view.lookAt(_cameraPos,_cameraPos + _cameraFront,_cameraUp);
        _shaderProgram->setUniformValue("view", view);

        // projection
        QMatrix4x4 projection;
        projection.perspective(
            45.0f, (float)((width() * retinaScale) / (height() * retinaScale)),
            0.1f, 100.0f);
        _shaderProgram->setUniformValue("projection",projection);

        // render
        // world space positions of our cubes
        QVector3D cubePositions[] = {
            QVector3D(0.0f, 0.0f, 0.0f),    QVector3D(2.0f, 5.0f, -15.0f),
            QVector3D(-1.5f, -2.2f, -2.5f), QVector3D(-3.8f, -2.0f, -12.3f),
            QVector3D(2.4f, -0.4f, -3.5f),  QVector3D(-1.7f, 3.0f, -7.5f),
            QVector3D(1.3f, -2.0f, -2.5f),  QVector3D(1.5f, 2.0f, -2.5f),
            QVector3D(1.5f, 0.2f, -1.5f),   QVector3D(-1.3f, 1.0f, -1.5f)};

        _vao->bind();
        for (auto i = 0;i< 10;i++) {
            QMatrix4x4 mat;
            mat.setToIdentity();
            mat.translate(cubePositions[i]);
            auto angle = 20.0f * i;
            mat.rotate(angle,QVector3D(1.0f,0.3f,0.5f));

            _shaderProgram->setUniformValue("model",mat);
            glDrawArrays(GL_TRIANGLES,0,36);
        }

        _context->swapBuffers(this);

    }

    void initialize() {
        initializeOpenGLFunctions();
        glEnable(GL_DEPTH_TEST);
        _shaderProgram = new QOpenGLShaderProgram(this);
        _shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                ":/vertex.glsl");
        _shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                ":/fragment.glsl");
        if (!_shaderProgram->link()) {
            qDebug() << _shaderProgram->log();
        }
        // data
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
        _vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
        _vbo->allocate(vertices, sizeof(vertices));

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // load and create texture
        _containerTexture =
            std::make_unique<QOpenGLTexture>(QImage(":/container.jpg"));
        _containerTexture->setWrapMode(QOpenGLTexture::Repeat);
        _containerTexture->setMinMagFilters(QOpenGLTexture::Linear,
                                            QOpenGLTexture::Linear);
        _containerTexture->generateMipMaps();

        _faceTexture = std::make_unique<QOpenGLTexture>(
            QImage(":/awesomeface.png").mirrored());
        _faceTexture->setWrapMode(QOpenGLTexture::Repeat);
        _faceTexture->setMinMagFilters(QOpenGLTexture::Linear,
                                       QOpenGLTexture::Linear);
        _faceTexture->generateMipMaps();

        _shaderProgram->bind();
        _shaderProgram->setUniformValue("texture1", 0);
        _shaderProgram->setUniformValue("texture2", 1);
    }

  private:
    QOpenGLContext* _context{nullptr};
    QOpenGLShaderProgram* _shaderProgram;
    QOpenGLVertexArrayObject* _vao;
    std::unique_ptr<QOpenGLBuffer> _vbo;
    std::unique_ptr<QOpenGLTexture> _containerTexture;
    std::unique_ptr<QOpenGLTexture> _faceTexture;
    QElapsedTimer _timer;
    QTimer *_repaintTimer;

    QVector3D _cameraPos{0.0f,0.0f,3.0f};
    QVector3D _cameraFront{0.0f,0.0f,-1.0f};
    QVector3D _cameraUp{0.0f,1.0f,0.0f};
};
int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    MyWindow window;
    QSurfaceFormat format;
    format.setSamples(16);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    window.setFormat(format);
    window.setTitle("LearnOpenGL");
    window.resize(640, 480);
    window.show();

    return app.exec();
}

#include "main.moc"