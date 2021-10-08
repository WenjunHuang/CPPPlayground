//
// Created by rick on 2021/9/7.
//
#include "window.h"

int main() {
  Run([](auto canvas){
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(0x80349A45);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(100);
    paint.setStrokeCap(SkPaint::kRound_Cap);
    canvas->scale(1,1.2f);
    canvas->drawPoint(64,96,paint);
    canvas->scale(.6f,.8f);
    paint.setColor(SK_ColorWHITE);
    canvas->drawPoint(106,120,paint);
  });

}