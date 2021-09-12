//
// Created by rick on 2021/9/12.
//
#include "window.h"
int main() {
  Run([](auto canvas) {
    SkColor colors[2] = {SK_ColorBLUE, SK_ColorYELLOW};
    SkPaint paint;
    paint.setShader(SkGradientShader::MakeRadial(
        SkPoint::Make(128.0f, 128.0f), 180.0f, colors, nullptr, 2,
        SkTileMode::kClamp, 0, nullptr));
    canvas->drawPaint(paint);
  },200,200);
}