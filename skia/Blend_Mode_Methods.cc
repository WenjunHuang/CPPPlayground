//
// Created by HUANG WEN JUN on 2021/9/7.
//
#include "window.h"

int main() {
  Run([](auto canvas) {
    SkPaint normal, blender;
    normal.setColor(0xFF58A889);
    blender.setColor(0xFF8958A8);
    canvas->clear(SK_ColorWHITE);

    for (SkBlendMode m :
         {SkBlendMode::kSrcOver, SkBlendMode::kSrcIn, SkBlendMode::kSrcOut}) {
      normal.setBlendMode(SkBlendMode::kSrcOver);
      canvas->drawOval(SkRect::MakeXYWH(30, 30, 30, 80), normal);
      blender.setBlendMode(m);
      canvas->drawOval(SkRect::MakeXYWH(10, 50, 80, 30), blender);
      canvas->translate(70, 70);
    }
  });
}