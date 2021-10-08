//
// Created by rick on 2021/9/12.
//
#include "window.h"
#include <skia/effects/Sk2DPathEffect.h>

int main() {
  // SkPath2DPathEffect: Stamp the specified path to fill the shape,
  // using the matrix to define the latice.
  Run([](auto canvas) {
    canvas->clear(SK_ColorYELLOW);
    SkScalar scale = 10.0f;
    SkPath path;
    static const int8_t pts[] = {2, 2, 1, 3, 0, 3, 2, 1, 3, 1, 4, 0, 4, 1,
                                 5, 1, 4, 2, 4, 3, 2, 5, 2, 4, 3, 3, 2, 3};
    path.moveTo(2 * scale, 3 * scale);
    for (size_t i = 0; i < sizeof(pts) / sizeof(pts[0]); i += 2) {
      path.lineTo(pts[i] * scale, pts[i + 1] * scale);
    }
    path.close();

    SkMatrix matrix = SkMatrix::Scale(4 * scale,4 *scale);
    SkPaint paint;
    paint.setPathEffect(SkPath2DPathEffect::Make(matrix,path));
    paint.setAntiAlias(true);
    SkRect bounds = SkRect::MakeXYWH(-4 * scale,-4 * scale,256,256);
    canvas->drawRect(bounds,paint);
  });
}
