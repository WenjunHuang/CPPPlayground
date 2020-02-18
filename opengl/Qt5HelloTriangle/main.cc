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
#include <QtGui/QtGui>

class TriangleWindow : public QWindow, private QOpenGLFunctions_3_3_Core {
    Q_OBJECT
  public:
    TriangleWindow() : _context{nullptr} {
        setSurfaceType(SurfaceType::OpenGLSurface);
    }

  protected:
    bool event(QEvent* event) override {
        switch (event->type()) {
        case QEvent::UpdateRequest: renderNow(); return true;
        default: return QWindow::event(event);
        }
    }
    void exposeEvent(QExposeEvent* event) override {
        Q_UNUSED(event);

        if (isExposed())
            renderNow();
    }

  private:
    void renderNow() {
        bool needInitialize = false;
        if (_context == nullptr) {
            _context = new QOpenGLContext(this);
            _context->setFormat(requestedFormat());
            _context->create();

            needInitialize = true;
        }

        _context->makeCurrent(this);
        if (needInitialize) {
            initializeOpenGLFunctions();
            _program = new QOpenGLShaderProgram(this);
            _program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                              ":/vertex.glsl");
            _program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                              ":/fragment.glsl");
            if (!_program->link()) {
                qDebug() << _program->log();
            }

            float vertices[] = {
                -0.9f,  -0.5f, -0.0f, // left
                0.0f,   -0.5f, 0.0f,  // right
                -0.45f, 0.5f,  0.0f   // top
            };

//            _vao = new QOpenGLVertexArrayObject(this);
//            _vao->bind();
            glGenVertexArrays(1,&_vao);
            glBindVertexArray(_vao);

            unsigned int VBO;
            glGenBuffers(1, &VBO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                         GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                  (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        // render
        const qreal retinaScale = devicePixelRatio();
        glViewport(0, 0, width() * retinaScale, height() * retinaScale);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        _program->bind();
        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        _context->swapBuffers(this);
    }

    QOpenGLContext* _context;
    QOpenGLShaderProgram* _program;
//    QOpenGLVertexArrayObject* _vao;
    unsigned int _vao;
};
int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QSurfaceFormat format;
    format.setSamples(16);
    TriangleWindow window;
    window.resize(640, 480);
    window.setFormat(format);
    window.show();

    return app.exec();
}

#include "main.moc"