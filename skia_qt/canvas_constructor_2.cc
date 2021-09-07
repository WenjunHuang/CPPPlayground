//
// Created by rick on 2021/9/7.
//
#include <skia/core/SkCanvas.h>

int main() {
  SkCanvas canvas(10, 20);
  canvas.clipRect(
      SkRect::MakeXYWH(30, 40, 5, 10));  // clip is outside canvas' device
  SkDebugf("canvas %s empty\n",
           canvas.getDeviceClipBounds().isEmpty() ? "is" : "is not");
}