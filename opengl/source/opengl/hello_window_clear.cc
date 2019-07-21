//
// Created by xxzyjy on 2019-01-02.
//
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

void frameBufferSizeCallback(GLFWwindow *window, int width, int height);

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

  auto window = glfwCreateWindow(SCR_WIDTH,
                                 SCR_HEIGHT,
                                 "LearnOpenGL",
                                 nullptr,
                                 nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    exit(-1);
  }

  while (!glfwWindowShouldClose(window)){
    processInput(window);

    // render
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window){
  if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window,true);
}

void frameBufferSizeCallback(GLFWwindow* window,int width,int height){
  glViewport(0,0,width,height);
}
