//
// Created by rick on 2021/9/7.
//

#include <GLFW/glfw3.h>
#include <skia/core/SkFont.h>
#include <algorithm>
#include <cstdlib>
#include "window.h"

class CanvasDestructor : public Window {
 protected:
  void OnDraw(SkCanvas* canvas) override {
    SkBitmap bitmap;
    bitmap.allocPixels(SkImageInfo::MakeN32Premul(200, 200));
    {
      SkCanvas offscreen(bitmap);
      SkPaint paint;
      SkFont font(nullptr, 100);
      offscreen.drawString("ABC", 20, 160, font, paint);

      SkRect layerBounds = SkRect::MakeXYWH(32, 32, 192, 192);
      offscreen.saveLayerAlpha(&layerBounds, 128);
      offscreen.clear(SK_ColorWHITE);
      offscreen.drawString("DEF", 20, 160, font, paint);
    }
    canvas->clear(SK_ColorWHITE);
    canvas->drawImage(SkImage::MakeFromBitmap(bitmap), 0, 0);
    canvas->flush();
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

  CanvasDestructor window;
  window.Run(xpos, ypos, width, height);
}