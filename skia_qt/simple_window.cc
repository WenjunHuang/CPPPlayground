//
// Created by HUANG WEN JUN on 2021/9/6.
//
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <cstdlib>

void error_callback(int error, const char* description) {}
static void key_callback(GLFWwindow* window,
                         int key,
                         int scancode,
                         int action,
                         int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}
int main() {
  if (!glfwInit()) {
    // Initialization failed
    exit(1);
  }
  glfwSetErrorCallback(error_callback);
  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  auto window = glfwCreateWindow(640, 480, "simple window", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);

  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glfwSwapInterval(1);



  glfwShowWindow(window);
  while (!glfwWindowShouldClose(window)) {
    int width,height;
    glfwGetFramebufferSize(window,&width,&height);
    glViewport(0,0,width,height);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
