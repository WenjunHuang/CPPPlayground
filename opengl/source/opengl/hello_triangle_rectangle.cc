#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

constexpr uint32_t SCR_WIDTH = 800;
constexpr uint32_t SCR_HEIGHT = 600;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  auto window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                 "LearnOpenGL",
                                 nullptr,
                                 nullptr);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

  Shader shader("vertex_hello_triangle_rectangle.glsl","fragment_hello_triangle_rectangle.glsl");
  shader.compile();
 

  // setup vertex data
  float triangleVertices[] = {
    -0.45f, 0.5f, 0.0f,
    -0.2f, -0.5f, 0.0f,
    -0.7f, -0.5f, 0.0f
  };

  float rectangleVertices[] = {
    0.2f, 0.5f, 0.0f,
    0.7f, 0.5f, 0.0f,
    0.7f, -0.5f, 0.0f,
    0.2f, -0.5f, 0.0f
  };
  uint32_t rectangleIndices[] = {
    0, 1, 2,
    2, 3, 0
  };

  uint32_t tVBO, rVBO, tVAO, rVAO, EBO;
  glGenVertexArrays(1, &tVAO);
  glGenVertexArrays(1, &rVAO);
  glGenBuffers(1, &tVBO);
  glGenBuffers(1, &rVBO);
  glGenBuffers(1, &EBO);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertext attributes(s).
  glBindVertexArray(tVAO);

  glBindBuffer(GL_ARRAY_BUFFER, tVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);

  // bind rectangle Vertex Array Object
  glBindVertexArray(rVAO);
  glBindBuffer(GL_ARRAY_BUFFER, rVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);

  // clear
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    shader.use();
    glBindVertexArray(tVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(rVAO);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &tVAO);
  glDeleteBuffers(1, &tVBO);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}