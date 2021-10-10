//
// Created by rick on 2021/9/7.
//
#include "window.h"
int main() {
  Run([](SkCanvas* canvas) {
    canvas->clear(SK_ColorWHITE);
    canvas->save();

    SkPaint paint;
    SkRect rect = {10, 20, 60, 120};
    canvas->translate(20, 20);
    canvas->drawRect(rect, paint);
    canvas->scale(2, 0.5f);
    paint.setColor(SK_ColorGRAY);
    canvas->drawRect(rect, paint);

    canvas->restore();
  });
}