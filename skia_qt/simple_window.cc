//
// Created by HUANG WEN JUN on 2021/9/6.
//

#include <GLFW/glfw3.h>

#include <cstdlib>
#include "window.h"

class SimpleWindow : public Window {
 protected:
  void OnDraw(SkCanvas* canvas) override {
    canvas->clear(SK_ColorGREEN);
    canvas->flush();
  }
};

int main() {
  if (!glfwInit()) {
    // Initialization failed
    exit(1);
  }
  auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  int width = (int)(vidmode->width * 0.75);
  int height = (int)(vidmode->height * 0.75);
  int xpos = std::max(0, (vidmode->width - width) / 2);
  int ypos = std::max(0, (vidmode->height - height) / 2);

  SimpleWindow window;

  window.Run(xpos, ypos, width, height);
}
