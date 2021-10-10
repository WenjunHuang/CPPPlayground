//
// Created by HUANG WEN JUN on 2021/9/8.
//
#include "window.h"
int main(){
  Run([](auto canvas){
    canvas->clear(SK_ColorWHITE);
    SkPaint paint;
    paint.setColor(SK_ColorRED);
    paint.setStrokeWidth(2.0f);

    canvas->drawLine(SkPoint(),SkPoint::Make(100,200),paint);
  });
}