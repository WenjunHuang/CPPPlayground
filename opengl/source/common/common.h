#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

constexpr uint32_t SCR_WIDTH = 800;
constexpr uint32_t SCR_HEIGHT = 600;

inline GLFWwindow *initOpenGL() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  auto window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                 "LearnOpenGL",
                                 nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, [](auto window, auto width, auto height) {
    glViewport(0, 0, width, height);
  });

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(-1);
  }

  return window;
}