//
// Created by rick on 2020/2/22.
//
#include <QtGui/QOpenGLFunctions_3_3_Core>
#include <QtGui/QtGui>

class ColorsWindow : public QOpenGLWindow, private QOpenGLFunctions_3_3_Core {
    Q_OBJECT
  protected:
    void initializeGL() override {
        initializeOpenGLFunctions();

        glEnable(GL_DEPTH_TEST);

        _lightingShader = new QOpenGLShaderProgram(this);
        _lightingShader->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                 ":/colors_vs.glsl");
        _lightingShader->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                 ":/colors_fs.glsl");
        if (!_lightingShader->link())
            qDebug() << _lightingShader->log();

        _lampShader = new QOpenGLShaderProgram(this);
        _lampShader->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                             ":/lamp_vs.glsl");
        _lampShader->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                             ":/lamp_fs.glsl");
        if (!_lampShader->link())
            qDebug() << _lampShader->log();

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
            0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

            -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
            0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
            0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

            -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f,
        };

        _cubeVAO = new QOpenGLVertexArrayObject(this);
        _cubeVAO->create();

        _vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        _vbo.create();
        _vbo.bind();
        _vbo.allocate(vertices, sizeof(vertices));
        _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

        _cubeVAO->bind();
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);

        // light's VAO,VBO stays the same
        _lightVAO = new QOpenGLVertexArrayObject(this);
        _lightVAO->create();
        _lightVAO->bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);

        _cameraPosition = QVector3D{0.2f,0.2f,3.0f};
        _cameraFront = QVector3D{0.0f,0.0f,-1.0f};
        _cameraUp = QVector3D{0.0f,1.0f,0.0f};
    }

    void paintGL() override {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        _lightingShader->bind();
        _lightingShader->setUniformValue("objectColor", QVector3D{1.0f, 0.5f, 0.31f});
        _lightingShader->setUniformValue("lightColor",QVector3D{1.0f,1.0f,1.0f});

        QMatrix4x4 projection;
        projection.perspective(45.0f,
            ((float)width())/height(),
            0.1f,
            100.0f);
        _lightingShader->setUniformValue("projection",projection);

        QMatrix4x4 view;
        view.setToIdentity();
        view.lookAt(_cameraPosition,_cameraPosition + _cameraFront,_cameraUp);
        _lightingShader->setUniformValue("view",view);

        QMatrix4x4 model;
        model.setToIdentity();
        _lightingShader->setUniformValue("model",model);

        // render the cube
        _cubeVAO->bind();
        glDrawArrays(GL_TRIANGLES,0,36);


        // also draw the lamp object
        _lampShader->bind();
        _lampShader->setUniformValue("projection",projection);
        _lampShader->setUniformValue("view",view);
        model.setToIdentity();
        model.translate(_lightPos);
        model.scale(0.2f);
        _lampShader->setUniformValue("model",model);

        _lightVAO->bind();
        glDrawArrays(GL_TRIANGLES,0,36);
    }

  private:
    QOpenGLShaderProgram* _lightingShader;
    QOpenGLShaderProgram* _lampShader;
    QOpenGLVertexArrayObject* _cubeVAO;
    QOpenGLVertexArrayObject* _lightVAO;

    QOpenGLBuffer _vbo;

    QVector3D _cameraPosition;
    QVector3D _cameraFront;
    QVector3D _cameraUp;
    QVector3D _lightPos{1.2f,1.0f,2.0f};
};

int main(int argc, char* argv[]) {
    QGuiApplication app{argc, argv};

    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setSamples(16);
    format.setProfile(QSurfaceFormat::CoreProfile);

    ColorsWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    return app.exec();
}

#include "main.moc"