#include <memory>

//
// Created by xxzyjy on 2019-01-04.
//
#include <QGuiApplication>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QTime>
#include <memory>
#include "OpenGLWindow.h"

class TransformationsWindow : public OpenGLWindow {
public:
  explicit TransformationsWindow(QWindow *parent = nullptr) : OpenGLWindow{parent} {
    setTitle("Transformation");
  }

  ~TransformationsWindow() override {}

  void initialize() override {
    _program = new QOpenGLShaderProgram(this);
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, "5.1.transform.vs");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, "5.1.transform.fs");
    _program->link();

    // initialize vao
    float vertices[] = {
      // positions               //texture coords
      0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f  // top left
    };
    uint32_t indices[] = {
      0, 1, 3, //first triangle
      1, 2, 3  //second triangle
    };

    _vao = new QOpenGLVertexArrayObject(this);
    _vao->create();
    _vao->bind();

    _vbo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
    _vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbo->create();

    _ebo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
    _ebo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    _ebo->create();

    // vbo
    _vbo->bind();
    _vbo->allocate(vertices, sizeof(vertices));

    _ebo->bind();
    _ebo->allocate(indices, sizeof(indices));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    _vao->release();

    // load and create texture
    QImage image;
    if (image.load("./assets/container.jpg")) {
      _texture1 = std::make_unique<QOpenGLTexture>(image);
      _texture1->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
      _texture1->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
      _texture1->setMinificationFilter(QOpenGLTexture::Linear);
      _texture1->setMagnificationFilter(QOpenGLTexture::Linear);
    } else {
      qDebug() << "Failed to load texture";
      exit(-1);
    }

    QImage image2;
    if (image2.load("./assets/awesomeface.png")) {
      _texture2 = std::make_unique<QOpenGLTexture>(image2.mirrored());
      _texture2->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
      _texture2->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
      _texture2->setMinificationFilter(QOpenGLTexture::Linear);
      _texture2->setMagnificationFilter(QOpenGLTexture::Linear);
    } else {
      qDebug() << "Failed to load texture";
      exit(-1);
    }

    _program->bind();
    _program->setUniformValue("texture1", 0);
    _program->setUniformValue("texture2", 1);

    _time = std::make_unique<QTime>();
    _time->start();
  }

  void render() override {
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _vao->bind();

    _texture1->bind(0);
    _texture2->bind(1);

    QMatrix4x4 mat4;
    mat4.translate(0.5f, -0.5f, 0.0f);
    mat4.rotate(_time->elapsed(), 0.0f, 0.0f, 1.0f);

    _program->bind();
    _program->setUniformValue("transform", mat4);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  }

private:
  QOpenGLVertexArrayObject *_vao;
  std::unique_ptr<QOpenGLBuffer> _vbo;
  std::unique_ptr<QOpenGLBuffer> _ebo;
  QOpenGLShaderProgram *_program;

  std::unique_ptr<QOpenGLTexture> _texture1;
  std::unique_ptr<QOpenGLTexture> _texture2;
  std::unique_ptr<QTime> _time;
};

int main(int argc, char **argv) {
  QGuiApplication app(argc, argv);
  TransformationsWindow window;
  window.resize(800, 600);
  window.show();
  window.setAnimating(true);

  return QGuiApplication::exec();
}