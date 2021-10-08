//
// Created by rick on 2021/9/7.
//
#include "window.h"

int main() {
  Run([](auto canvas) {
    canvas->clear(SK_ColorWHITE);
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(10);
    paint.setColor(0x80349A45);
    const SkPoint points[] = {{32, 16}, {48, 48}, {16, 32}};
    const SkPaint::Join join[] = {SkPaint::kRound_Join, SkPaint::kMiter_Join,
                                  SkPaint::kBevel_Join};
    int joinIndex = 0;
    SkPath path;
    path.addPoly(points, 3, false);
    for (const auto cap :
         {SkPaint::kRound_Cap, SkPaint::kSquare_Cap, SkPaint::kButt_Cap}) {
      paint.setStrokeCap(cap);
      paint.setStrokeJoin(join[joinIndex++]);
      for(const auto mode:{SkCanvas::kPoints_PointMode,
           SkCanvas::kLines_PointMode,
           SkCanvas::kPolygon_PointMode}) {
        canvas->drawPoints(mode,3,points,paint);
        canvas->translate(64,0);
      }
      canvas->drawPath(path,paint);
      canvas->translate(-192,64);
    }
  });
}