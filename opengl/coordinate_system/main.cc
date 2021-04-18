//
// Created by rick on 2020/2/18.
//

#include <QtGui/QOpenGLFunctions_3_3_Core>
#include <QtGui/QtGui>

class Window : public QWindow, private QOpenGLFunctions_3_3_Core {
  public:
    Window() { setSurfaceType(SurfaceType::OpenGLSurface); }

  protected:
    void exposeEvent(QExposeEvent* event) override {
        if (isExposed())
            renderNow();
    }

    bool event(QEvent* event) override {
        switch (event->type()) {
        case QEvent::UpdateRequest: {
            renderNow();
            return true;
        }
        default:
            return QWindow::event(event);
        }
    }

  private:
    void initialize() {
        initializeOpenGLFunctions();

        _shader = new QOpenGLShaderProgram(this);
        _shader->create();
        _shader->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                         ":/vertex.glsl");
        _shader->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":/fragment.glsl");
        if (!_shader->link()) {
            qDebug() << _shader->log();
        }

        _vao = new QOpenGLVertexArrayObject(this);
        _vao->create();
        _vao->bind();

        float vertices[] = {// positions     // texture coords
                            0.5f, 0.5f,  0.0f, 1.0f,  1.0f,  0.5f, -0.5f,
                            0.0f, 1.0f,  0.0f, -0.5f, -0.5f, 0.0f, 0.0f,
                            0.0f, -0.5f, 0.5f, 0.0f,  0.0f,  1.0f};
        _vbo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
        _vbo->create();
        _vbo->allocate(vertices, sizeof(vertices));
        _vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        // load texture
        _container = std::make_unique<QOpenGLTexture>(QImage(":/container.jpg"));

        _vao->release();
        _shader->release();
    }

    void renderNow() {}

  private:
    QOpenGLContext* _context;
    QOpenGLShaderProgram* _shader;
    QOpenGLVertexArrayObject* _vao;

    std::unique_ptr<QOpenGLBuffer> _vbo;
    std::unique_ptr<QOpenGLBuffer> _ebo;
    std::unique_ptr<QOpenGLTexture> _container;
    std::unique_ptr<QOpenGLTexture> _face;
};
int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    return app.exec();
}