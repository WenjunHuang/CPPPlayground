//
// Created by rick on 2021/9/7.
//

#include "window.h"
#include <skia/core/SkFont.h>
class MyWindow: public Window {
 protected:
  void OnDraw(SkCanvas *canvas) override {
    SkPaint paint;
    SkFont font(nullptr,100);
    canvas->drawString("ABC",20,160,font,paint);

    SkRect layerBounds = SkRect::MakeXYWH(32,32,192,192);
    canvas->saveLayerAlpha(&layerBounds,128);
    canvas->clear(SK_ColorWHITE);
    canvas->drawString("DEF",20,160,font,paint);

    SkImageInfo imageInfo;
    size_t rowBytes;
    SkIPoint origin;
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