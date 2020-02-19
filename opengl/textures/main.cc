//
// Created by rick on 2020/2/17.
//

#include <QEvent>
#include <QGuiApplication>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QWindow>
#include <QtCore/qfile.h>
#include <iostream>

#include <QKeyEvent>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QElapsedTimer>

// settings
constexpr unsigned int kScrWidth  = 800;
constexpr unsigned int kScrHeight = 600;

class TextureWindow : public QWindow, private QOpenGLFunctions_3_3_Core {
    Q_OBJECT
  public:
    TextureWindow() {
        setSurfaceType(SurfaceType::OpenGLSurface);
        _time.start();
    }

  protected:
    bool event(QEvent* event) override {
        switch (event->type()) {
        case QEvent::UpdateRequest: {
            renderNow();
            return true;
        }
        case QEvent::KeyPress: {
            auto keyEvent = dynamic_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Up) {
                _value = std::min(1.0f, _value + 0.1f);
                requestUpdate();
            } else if (keyEvent->key() == Qt::Key_Down) {
                _value = std::max(0.0f, _value - 0.1f);
                requestUpdate();
            } else
                return QWindow::event(event);
        }
        default: return QWindow::event(event);
        }
    }

    void exposeEvent(QExposeEvent* event) override {
        Q_UNUSED(event);

        if (isExposed())
            renderNow();
    }

    void renderNow() {
        bool needInitialize = false;
        if (!_context) {
            _context = new QOpenGLContext(this);
            _context->setFormat(requestedFormat());
            _context->create();
            needInitialize = true;
        }
        _context->makeCurrent(this);
        if (needInitialize) {
            initializeOpenGLFunctions();
            initialize();
        }

        const qreal retinaScale = devicePixelRatio();
        glViewport(0, 0, width() * retinaScale, height() * retinaScale);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind texture
        glActiveTexture(GL_TEXTURE1);
        _texture->bind();
        glActiveTexture(GL_TEXTURE2);
        _texture1->bind();
        //        glBindTexture(GL_TEXTURE_2D, _texture);

        // render container
        _program->bind();
        _program->setUniformValue("texture1", 1);
        _program->setUniformValue("texture2", 2);
        _program->setUniformValue("value", _value);

        QMatrix4x4 matrix;
        matrix.translate(0.5f,-0.5f,0.0f);
        matrix.rotate(_time.elapsed(),QVector3D(0.0f,0.0f,1.0f));
        _program->setUniformValue("transform",matrix);

        _vao->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        _context->swapBuffers(this);

        requestUpdate();
    }

    void initialize() {
        _program = new QOpenGLShaderProgram(this);
        _program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/vertex.glsl");
        _program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/fragment.glsl");
        if (!_program->link()) {
            qDebug() << _program->log();
        }

        // build and compile our shader program
        // shader
        float vertices[] = {
            // positions       // colors         // texture coords
            0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
            0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
        };

        unsigned int indices[] = {0, 1, 3, 1, 2, 3};

        _vao = new QOpenGLVertexArrayObject(this);
        _vao->create();
        _vao->bind();

        _vbo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
        _vbo->create();
        _vbo->bind();
        _vbo->allocate(vertices, sizeof(vertices));

        _ebo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
        _ebo->create();
        _ebo->bind();
        _ebo->allocate(indices, sizeof(indices));
        //        glGenBuffers(1, &VBO);
        //        glGenBuffers(1, &EBO);

        //        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
        //                     GL_STATIC_DRAW);
        //
        //        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
        //        indices,
        //                     GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);

        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // load and create a texture
        //        glGenTextures(1, &_texture);
        //        glBindTexture(GL_TEXTURE_2D, _texture);
        // set the texture wrapping parameters
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        //        GL_LINEAR); glTexParameteri(GL_TEXTURE_2D,
        //        GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load and create a texture
        QImage image(":/container.jpg");
        qDebug() << "image width:" << image.width()
                 << ",height:" << image.height();
        _texture = std::make_unique<QOpenGLTexture>(image);
        _texture->generateMipMaps();

        _texture->setMinificationFilter(QOpenGLTexture::Linear);
        _texture->setMagnificationFilter(QOpenGLTexture::Linear);
        _texture->setWrapMode(QOpenGLTexture::DirectionS,
                              QOpenGLTexture::Repeat);
        _texture->setWrapMode(QOpenGLTexture::DirectionT,
                              QOpenGLTexture::Repeat);

        _texture1 = std::make_unique<QOpenGLTexture>(
            QImage(":/awesomeface.png").mirrored());
        _texture1->generateMipMaps();

        qDebug() << _texture->textureId() << " " << _texture1->textureId();
        //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(),
        //        image.height(), 0,
        //                     GL_RGB, GL_UNSIGNED_BYTE, image.);
        //        glGenerateMipmap(GL_TEXTURE_2D);
    }

  private:
    QOpenGLContext* _context{nullptr};
    QOpenGLVertexArrayObject* _vao;
    QOpenGLShaderProgram* _program{nullptr};

    std::unique_ptr<QOpenGLBuffer> _vbo;
    std::unique_ptr<QOpenGLBuffer> _ebo;
    std::unique_ptr<QOpenGLTexture> _texture;
    std::unique_ptr<QOpenGLTexture> _texture1;

    QElapsedTimer _time;

    float _value{0.2};
    //    unsigned int _texture;
};

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QSurfaceFormat format;
    format.setSamples(16);
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    TextureWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();
    return app.exec();
}
#include "main.moc"
