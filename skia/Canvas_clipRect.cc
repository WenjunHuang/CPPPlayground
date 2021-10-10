//
// Created by rick on 2021/9/7.
//

#include "window.h"

int main() {
  Run(
      [](SkCanvas* canvas) {
        canvas->clear(SK_ColorWHITE);
        canvas->rotate(10);
        SkPaint paint;
        paint.setAntiAlias(true);
        for (auto alias : {false, true}) {
          canvas->save();
          canvas->clipRect(SkRect::MakeWH(90, 80), SkClipOp::kIntersect, alias);
          canvas->drawCircle(100, 60, 60, paint);
          canvas->restore();
          canvas->translate(80, 0);
        }
      },
      200, 200);
}