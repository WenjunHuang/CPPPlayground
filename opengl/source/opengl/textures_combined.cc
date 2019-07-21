//
// Created by rick on 19-1-2.
//

#include <iostream>
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QWindow>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QImage>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include "OpenGLWindow.h"

class TexturesCombinedWindow : public OpenGLWindow {
public:
  explicit TexturesCombinedWindow(QWindow *parent = nullptr) : OpenGLWindow{parent} {}

  ~TexturesCombinedWindow() override {}

  void render() override {

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind textures on corresponding texture units
//    glActiveTexture(GL_TEXTURE0);
    _containerTex->bind(0);
//    glBindTexture(GL_TEXTURE_2D, texture1);
    _faceTex->bind(1);
//    glBindTexture(GL_TEXTURE_2D, texture2);

    // render container
    _program->bind();
//    glBindVertexArray(VAO);
    _vao->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  }

  void initialize() override {
    _program = new QOpenGLShaderProgram(this);
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, "4.2.texture.vs");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, "4.2.texture.fs");
    _program->link();

    float vertices[] = {
      // positions        // colors         // texture coords
      0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right corner
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right corner
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left corner
      -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left corner
    };

    uint32_t indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
    };

    _vao = new QOpenGLVertexArrayObject;
    _vao->create();
    _vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    _vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbo->create();
    _ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    _ebo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    _ebo->create();

    _vao->bind();

    _vbo->bind();
    _vbo->allocate(vertices, sizeof(vertices));

    _ebo->bind();
    _ebo->allocate(indices, sizeof(indices));

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    QImage containerImage;
    if (containerImage.load("./assets/container.jpg")) {
      _containerTex = new QOpenGLTexture(containerImage, QOpenGLTexture::GenerateMipMaps);
      _containerTex->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
      _containerTex->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
      _containerTex->setMinificationFilter(QOpenGLTexture::Linear);
      _containerTex->setMagnificationFilter(QOpenGLTexture::Linear);
    } else {
      qDebug() << "Failed to load texture";
      exit(-1);
    }


    // texture 2
    QImage image;
    if (image.load("./assets/awesomeface.png")) {
      _faceTex = new QOpenGLTexture(image.mirrored());
      _faceTex->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
      _faceTex->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
      _faceTex->setMinificationFilter(QOpenGLTexture::Linear);
      _faceTex->setMagnificationFilter(QOpenGLTexture::Linear);
    } else {
      qDebug() << "Failed to load texture";
      exit(-1);
    }
    _program->bind();
    _program->setUniformValue("texture1", 0);
    _program->setUniformValue("texture2", 1);
  }

private:
  QOpenGLVertexArrayObject *_vao;
  QOpenGLBuffer *_vbo;
  QOpenGLBuffer *_ebo;
  QOpenGLShaderProgram *_program;
  QOpenGLTexture *_containerTex;
  QOpenGLTexture *_faceTex;
};

int main(int argc, char **argv) {
  QGuiApplication app(argc, argv);

  TexturesCombinedWindow window;
  window.resize(800, 600);
  window.show();

  return app.exec();

//  auto window = initOpenGL();
//  Shader ourShader("4.2.texture.vs", "4.2.texture.fs");
//  ourShader.compile();
//
//
//
//  // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
//  ourShader.use();
//  ourShader.setInt("texture1", 0);
//  ourShader.setInt("texture2", 1);
//
//  // render loop
//  while (!glfwWindowShouldClose(window)) {
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//      glfwSetWindowShouldClose(window, true);
//
//    // render
//    glfwPollEvents();
//  }
//
//  glDeleteVertexArrays(1, &VAO);
//  glDeleteBuffers(1, &VBO);
//  glDeleteBuffers(1, &EBO);
//
//  glfwTerminate();
//  return 0;
}

//void processInput(GLFWwindow *window) {
//}
//
//void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
//  glViewport(0, 0, width, height);
//}
