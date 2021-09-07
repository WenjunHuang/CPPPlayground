#include "window.h"
//
// Created by rick on 2021/9/7.
//
class MyWindow : public Window {
 protected:
  void OnDraw(SkCanvas *canvas) override {
    if (canvas->accessTopLayerPixels(nullptr,nullptr))
      canvas->clear(SK_ColorRED);
    else
      canvas->clear(SK_ColorBLUE);
  }

};
int main() {
  if (!glfwInit()) {
    // Initialization failed
    exit(1);
  }
  auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  int width = 200;
  int height = 200;
  int xpos = std::max(0, (vidmode->width - width) / 2);
  int ypos = std::max(0, (vidmode->height - height) / 2);

  MyWindow window;
  window.Run(xpos, ypos, width, height);
}