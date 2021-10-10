//
// Created by rick on 2021/9/7.
//
#include "window.h"
#include <skia/core/SkVertices.h>

int main() {
  Run([](auto canvas) {
    canvas->clear(SK_ColorWHITE);
    SkPaint paint;
    SkPoint points[] = {{0, 0}, {250, 0}, {100, 100}, {0, 250}};
    SkPoint texs[] = {{0, 0}, {0, 250}, {250, 250}, {250, 0}};
    SkColor colors[] = {SK_ColorRED,SK_ColorBLUE,SK_ColorYELLOW,SK_ColorCYAN};

    paint.setShader(SkGradientShader::MakeLinear(points,colors,nullptr,4,SkTileMode::kClamp));

    auto vertices = SkVertices::MakeCopy(SkVertices::kTriangleFan_VertexMode,
                                         SK_ARRAY_COUNT(points),
                                         points,
                                         texs,colors);
    canvas->drawVertices(vertices,SkBlendMode::kDarken,paint);

  });
}