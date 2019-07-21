//
// Created by xxzyjy on 2019-01-04.
//

#include <QGuiApplication>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "OpenGLWindow.h"

class CoordinateSystemsMultiple: public OpenGLWindow {
public:
  explicit CoordinateSystemsMultiple(QWindow* parent = nullptr) : OpenGLWindow{parent}{

  }

  void initialize() override {
    _program = new QOpenGLShaderProgram;
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex,"6.3.coordinate_systems.vs");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment,"6.3.coordinate_systems.fs");
    _program->link();
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
    // world space positions of our cubes
    QVector3D cubePositions[] = {
      QVector3D(0.0f,  0.0f,  0.0f),
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

    _vao = new QOpenGLVertexArrayObject;
    _vao->create();
    _vao->bind();

    _vbo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
    _vbo->create();
  }

private:
  QOpenGLShaderProgram *_program;
  QOpenGLVertexArrayObject *_vao;

  std::unique_ptr<QOpenGLBuffer> _vbo;
  std::unique_ptr<QOpenGLBuffer> _ebo;

};

int main(int argc,char** argv){
  QGuiApplication app(argc,argv);
  CoordinateSystemsMultiple window;
  window.resize(800,600);
  window.show();

  return QGuiApplication::exec();
}