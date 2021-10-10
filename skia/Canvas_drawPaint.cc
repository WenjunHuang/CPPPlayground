//
// Created by rick on 2021/9/7.
//

#include "window.h"

int main() {
  Run([](SkCanvas* canvas) {
    canvas->clear(SK_ColorWHITE);
    SkColor colors[] = {SK_ColorRED, SK_ColorGREEN, SK_ColorBLUE};
    SkScalar pos[] = {0, SK_Scalar1 / 2, SK_Scalar1};
    SkPaint paint;
    paint.setShader(SkGradientShader::MakeSweep(256, 256, colors, pos,
                                                SK_ARRAY_COUNT(colors)));
    canvas->drawPaint(paint);
  });
}