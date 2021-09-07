//
// Created by rick on 2021/9/7.
//
#include "window.h"
int main() {
  Run(
      [](SkCanvas* canvas) {
        canvas->clear(SK_ColorWHITE);
        SkPaint paint;
        paint.setAntiAlias(true);
        paint.setColor(0x8055aaff);
        SkRRect oval;
        oval.setOval({10,20,90,100});
        canvas->clipRRect(oval,SkClipOp::kIntersect, true);
        canvas->drawCircle(70,100,60,paint);

      },
      300, 300);
}