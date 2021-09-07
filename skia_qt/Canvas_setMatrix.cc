//
// Created by rick on 2021/9/7.
//
#include "window.h"
#include <skia/core/SkFont.h>

int main(){
  Run([](SkCanvas* canvas){
    canvas->save();
    canvas->clear(SK_ColorWHITE);
    SkPaint paint;
    SkFont font;
    canvas->scale(4,6);
    canvas->drawString("true",2,10,font,paint);

    SkMatrix matrix;
    matrix.setScale(2.8f,6);
    canvas->setMatrix(matrix);
    canvas->drawString("consequences",2,20,font,paint);

    canvas->restore();

  });

}