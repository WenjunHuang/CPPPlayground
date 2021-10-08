//
// Created by rick on 2021/9/7.
//

#include <skia/core/SkFont.h>
#include "window.h"

int main() {
  Run(
      [](SkCanvas* canvas) {
        canvas->clear(SK_ColorWHITE);
        SkPaint paint;
        SkFont font(nullptr, 80);
        font.setScaleX(.3f);

        SkRect rect[] = {{10, 20, 90, 110}, {40, 130, 140, 180}};
        canvas->drawRect(rect[0],paint);
        canvas->drawRect(rect[1],paint);

        paint.setColor(SK_ColorWHITE);
        canvas->drawString("Here",rect[0].fLeft + 10,rect[0].fBottom - 10,font,paint);
        canvas->concat(SkMatrix::MakeRectToRect(rect[0],rect[1],SkMatrix::ScaleToFit::kFill_ScaleToFit));
        canvas->drawString("There",rect[0].fLeft+ 10,rect[0].fBottom - 10,font,paint);
      },
      300, 300);
}