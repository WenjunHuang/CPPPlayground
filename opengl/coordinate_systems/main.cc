//
// Created by HUANG WEN JUN on 2020/2/19.
//
#include <QOpenGLFunctions_3_3_Core>
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <memory>

class Window : public QWindow, private QOpenGLFunctions_3_3_Core {
    Q_OBJECT
  public:
    Window() { setSurfaceType(SurfaceType::OpenGLSurface); _time.start();}

    bool event(QEvent* event) override {
        switch (event->type()) {
        case QEvent::UpdateRequest: renderNow(); return true;
        default: return QWindow::event(event);
        }
    }

    void exposeEvent(QExposeEvent* event) override {
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
        if (needInit)
            initialize();

        const qreal retinaScale = devicePixelRatio();
        glViewport(0, 0, width() * retinaScale, height() * retinaScale);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        _containerTexture->bind();
        glActiveTexture(GL_TEXTURE1);
        _faceTexture->bind();

        // active shader
        _shaderProgram->bind();



        // render container
        QVector3D cubePositions[] = {
            QVector3D( 0.0f,  0.0f,  0.0f),
            QVector3D( 2.0f,  5.0f, -15.0f),
            QVector3D(-1.5f, -2.2f, -2.5f),
            QVector3D(-3.8f, -2.0f, -12.3f),
            QVector3D( 2.4f, -0.4f, -3.5f),
            QVector3D(-1.7f,  3.0f, -7.5f),
            QVector3D( 1.3f, -2.0f, -2.5f),
            QVector3D( 1.5f,  2.0f, -2.5f),
            QVector3D( 1.5f,  0.2f, -1.5f),
            QVector3D(-1.3f,  1.0f, -1.5f)
        };
        _vao->bind();
        int i = 0;
        for (const auto& position:cubePositions) {
            // create transform
            QMatrix4x4 model, view, projection;
            model.setToIdentity();
            model.translate(position);
            auto angle = 20.0f * (i++);
            model.rotate(angle, 0.5f, 1.0f, 0.0f);

            view.setToIdentity();
            view.translate(0.0f, 0.0f, -3.0f);

            projection.setToIdentity();
            auto viewSize = size();
            projection.perspective(
                45, (float)(width() * retinaScale / height() * retinaScale), 0.1f,
                100.0f);

            // retrieve the matrix uniform locations
            auto modelLoc = _shaderProgram->uniformLocation("model");
            auto viewLoc  = _shaderProgram->uniformLocation("view");

            // pass them to the shaders
            _shaderProgram->setUniformValue(modelLoc, model);
            _shaderProgram->setUniformValue(viewLoc, view);
            _shaderProgram->setUniformValue("projection", projection);

            glDrawArrays(GL_TRIANGLES,0,36);
        }
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        _context->swapBuffers(this);

//        requestUpdate();
    }

    void initialize() {
        initializeOpenGLFunctions();

        glEnable(GL_DEPTH_TEST);

        // builder shaders
        _shaderProgram = new QOpenGLShaderProgram(this);
        _shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                ":/vertex.glsl");
        _shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                ":/fragment.glsl");
        if (!_shaderProgram->link())
            qDebug() << _shaderProgram->log();

        // set up vertex data
//        float vertices[] = {
//            // positions      // texture coords
//            0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top right
//            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
//            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
//            -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top left
//        };
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        constexpr auto stride  = 5 * sizeof(float);
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        _vao = new QOpenGLVertexArrayObject(this);
        _vao->create();
        _vao->bind();

        _vbo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
        _vbo->create();
        _vbo->bind();
        _vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
        _vbo->allocate(vertices, sizeof(vertices));

        _ebo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
        _ebo->create();
        _ebo->bind();
        _ebo->setUsagePattern(QOpenGLBuffer::StaticDraw);
        _ebo->allocate(indices, sizeof(indices));

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // load and create a texture
        _containerTexture =
            std::make_unique<QOpenGLTexture>(QImage(":/container.jpg"));
        _containerTexture->create();
        _containerTexture->setWrapMode(QOpenGLTexture::DirectionS,
                                       QOpenGLTexture::Repeat);
        _containerTexture->setWrapMode(QOpenGLTexture::DirectionT,
                                       QOpenGLTexture::Repeat);
        _containerTexture->setMinMagFilters(QOpenGLTexture::Linear,
                                            QOpenGLTexture::Linear);

        _faceTexture = std::make_unique<QOpenGLTexture>(
            QImage(":/awesomeface.png").mirrored());
        _faceTexture->create();
        _faceTexture->setWrapMode(QOpenGLTexture::DirectionS,
                                       QOpenGLTexture::Repeat);
        _faceTexture->setWrapMode(QOpenGLTexture::DirectionT,
                                       QOpenGLTexture::Repeat);
        _faceTexture->setMinMagFilters(QOpenGLTexture::Linear,
                                            QOpenGLTexture::Linear);
        _faceTexture->generateMipMaps();

        _shaderProgram->bind();
        _shaderProgram->setUniformValue("texture1", 0);
        _shaderProgram->setUniformValue("texture2", 1);

    }

  private:
    QElapsedTimer _time;
    QOpenGLContext* _context{nullptr};
    QOpenGLVertexArrayObject* _vao{nullptr};
    QOpenGLShaderProgram* _shaderProgram{nullptr};

    std::unique_ptr<QOpenGLBuffer> _vbo;
    std::unique_ptr<QOpenGLBuffer> _ebo;
    std::unique_ptr<QOpenGLTexture> _containerTexture;
    std::unique_ptr<QOpenGLTexture> _faceTexture;
};

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    Window window;
    window.setFormat(format);
    window.resize(800, 600);
    window.setTitle("LearnOpenGL");
    window.show();

    return app.exec();
}

#include "main.moc"