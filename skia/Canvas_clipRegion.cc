//
// Created by rick on 2021/9/7.
//

#include <skia/core/SkRegion.h>
#include "window.h"
int main() {
  Run(
      [](SkCanvas* canvas) {
        canvas->clear(SK_ColorWHITE);
        SkPaint paint;
        paint.setAntiAlias(true);
        SkIRect iRect = {30, 40, 120, 130};
        SkRegion region(iRect);

        canvas->rotate(10);
        canvas->save();
        canvas->clipRegion(region, SkClipOp::kIntersect);//deviceRgn is unaffected by SkMatrix.
        canvas->drawCircle(50, 50, 45, paint);
        canvas->restore();

        canvas->translate(100, 100);
        canvas->clipRect(SkRect::Make(iRect), SkClipOp::kIntersect);
        canvas->drawCircle(50, 50, 45, paint);
      },
      300, 300);
}