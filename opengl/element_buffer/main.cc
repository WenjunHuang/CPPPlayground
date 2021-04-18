//
// Created by rick on 2021/4/17.
//
#include <QGuiApplication>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QSurfaceFormat>
#include <QWindow>
#include <qopenglwindow.h>

class ElementBufferWindow : public QWindow,private QOpenGLFunctions_4_5_Core {
    Q_OBJECT
  public:
    ElementBufferWindow() : _context{nullptr} {
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
        bool needInitialization = false;
        if (_context == nullptr) {
            needInitialization = true;
            _context = new QOpenGLContext{this};
            _context->setFormat(requestedFormat());
            _context->create();
        }
        _context->makeCurrent(this);
        if (needInitialization) {
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
                0.5f,  0.5f, 0.0f,  // top right
                0.5f, -0.5f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f,  // bottom left
                -0.5f,  0.5f, 0.0f   // top left
            };
            float vertices1[] = {
                0.9f,  0.9f, 0.0f,  // top right
                0.9f, -0.9f, 0.0f,  // bottom right
                -0.9f, -0.9f, 0.0f,  // bottom left
                -0.9f,  0.9f, 0.0f   // top left
            };
            unsigned int indices[] = {  // note that we start from 0!
                0, 1, 3,   // first triangle
                1, 2, 3    // second triangle
            };

            glGenVertexArrays(1,&_vao);
            glBindVertexArray(_vao);

            // generate and set vertex buffer
            unsigned int vbo;
            glGenBuffers(1,&vbo);
            glBindBuffer(GL_ARRAY_BUFFER,vbo);
            glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

            // generate and set element buffer
            unsigned int ebo;
            glGenBuffers(1,&ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

            // set the vertex attributes pointers
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),nullptr);
            glEnableVertexAttribArray(0);


            // another vertex buffer
            glGenBuffers(1,&vbo);
            glBindBuffer(GL_ARRAY_BUFFER,vbo);
            glBufferData(GL_ARRAY_BUFFER,sizeof(vertices1),vertices1,GL_STATIC_DRAW);

            glGenBuffers(1,&ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

            glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),nullptr);
            glEnableVertexAttribArray(1);
        }

        // render
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        const qreal retinaScale = devicePixelRatio();
        glViewport(0, 0, width() * retinaScale, height() * retinaScale);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        _program->bind();
        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,0);
        _context->swapBuffers(this);
    }
    QOpenGLContext* _context;
    QOpenGLShaderProgram* _program;
    //    QOpenGLVertexArrayObject* _vao;
    unsigned int _vao;
};
int main(int argc, char* argv[]) {
    QGuiApplication app{argc, argv};
    QSurfaceFormat format;
    format.setSamples(16);
    format.setVersion(4, 6);
    format.setProfile(QSurfaceFormat::CoreProfile);

    ElementBufferWindow window;
    window.resize(640, 480);
    window.setFormat(format);
    window.show();
    return app.exec();
}

#include "main.moc"