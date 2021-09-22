//
// Created by rick on 2021/9/7.
//

#pragma once
#include <GLFW/glfw3.h>
#include <glad.h>
#include <cassert>
#include <functional>
#include <iostream>

class Window {
 public:
  void Run(int x, int y, size_t width, size_t height) {
    CreateWindow(x, y, width, height);
    Loop();

    glfwDestroyWindow(window_);
    glfwTerminate();
  }

 protected:
  virtual void OnDraw() = 0;
  virtual void OnKey(int key, int action) {}
  virtual void OnScroll(double xoffset, double yoffset){};

 private:
  void CreateWindow(int x, int y, size_t width, size_t height) {
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = glfwCreateWindow(width, height, "skia demo", nullptr, nullptr);
    if (window_ == nullptr) {
      throw std::domain_error("Failed to create the GLFW window");
    }

    glfwSetWindowUserPointer(window_, this);
    glfwSetKeyCallback(window_, &WindowKeyCallback);
    glfwSetWindowPos(window_, x, y);
    UpdateDimensions();
    xpos_ = width / 2;
    ypos_ = height / 2;

    glfwMakeContextCurrent(window_);
    //    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
    glfwShowWindow(window_);
  }

  void UpdateDimensions() {
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);

    float xscale, yscale;
    glfwGetWindowContentScale(window_, &xscale, &yscale);

    assert(xscale == yscale);

    width_ = static_cast<size_t>(width / xscale);
    height_ = static_cast<size_t>(height / xscale);
    dpi_ = xscale;
  }

  void Draw() {
    OnDraw();
    glfwSwapBuffers(window_);
  }

  void Loop() {
    glfwSetWindowSizeCallback(window_, &WindowResized);
    glfwSetCursorPosCallback(window_, &WindowCursorPosCallback);
    glfwSetScrollCallback(window_, &WindowScrollCallback);
    glfwSetKeyCallback(window_, &WindowKeyCallback);

    // glad:load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << '\n';
      return;
    }

    while (!glfwWindowShouldClose(window_)) {
      Draw();
      glfwPollEvents();
    }
  }

  void Scroll(double xoffset, double yoffset) {
    OnScroll(xoffset * dpi_, yoffset * dpi_);
  }

 protected:
  GLFWwindow* window_;
  double xpos_;
  double ypos_;
  size_t width_;
  size_t height_;
  double dpi_;

  static void WindowKeyCallback(GLFWwindow* window,
                                int key,
                                int scancode,
                                int action,
                                int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
      glfwSetWindowShouldClose(window, true);
    auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->OnKey(key, action);
  }

  static void WindowResized(GLFWwindow* window, int width, int height) {
    auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->UpdateDimensions();
    w->Draw();
  }

  static void WindowCursorPosCallback(GLFWwindow* window,
                                      double xpos,
                                      double ypos) {
    auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
#ifdef __APPLE__
    w->xpos_ = xpos;
    w->ypos_ = ypos;
#else
    w->xpos_ = xpos / w->dpi_;
    w->ypos_ = ypos / w->dpi_;
#endif
  }

  static void WindowScrollCallback(GLFWwindow* window,
                                   double xoffset,
                                   double yoffset) {
    auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->Scroll(xoffset, yoffset);
  }
};

using OnDrawCallback = std::function<void()>;
class DefaultWindow : public Window {
 public:
  using CallbackType = OnDrawCallback;
  DefaultWindow(OnDrawCallback callback) : draw_callback_(callback) {}

 protected:
  void OnDraw() override { draw_callback_(); }

 private:
  OnDrawCallback draw_callback_;
};

using OnDrawWithSizeCallback = std::function<void(int, int)>;
class DefaultWithSizeWindow : public Window {
 public:
  using CallbackType = OnDrawWithSizeCallback;
  DefaultWithSizeWindow(OnDrawWithSizeCallback callback)
      : draw_callback_(callback) {}

 protected:
  void OnDraw() override { draw_callback_(width_, height_); }

 private:
  OnDrawWithSizeCallback draw_callback_;
};

template <typename T>
void RunTemplate(typename T::CallbackType onDraw, int width, int height) {
  if (!glfwInit()) {
    // Initialization failed
    exit(1);
  }
  auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  int w = width == -1 ? (int)(vidmode->width * 0.75) : width;
  int h = height == -1 ? (int)(vidmode->height * 0.75) : height;
  int xpos = std::max(0, (vidmode->width - width) / 2);
  int ypos = std::max(0, (vidmode->height - height) / 2);

  T window(onDraw);
  window.Run(xpos, ypos, w, h);
}

inline void Run(OnDrawCallback onDraw, int width = -1, int height = -1) {
  RunTemplate<DefaultWindow>(onDraw, width, height);
}

inline void Run(OnDrawWithSizeCallback onDraw,
                int width = -1,
                int height = -1) {
  RunTemplate<DefaultWithSizeWindow>(onDraw, width, height);
}

