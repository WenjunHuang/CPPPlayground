//
// Created by rick on 2021/9/12.
//

#include "window.h"
#include <skia/core/SkMaskFilter.h>
#include <skia/core/SkTextBlob.h>

int main(){

  Run([](auto canvas){
    canvas->clear(SK_ColorWHITE);
    SkPaint paint;
    paint.setMaskFilter(SkMaskFilter::MakeBlur(kNormal_SkBlurStyle,5.0f));
    auto blob = SkTextBlob::MakeFromString("Skia",SkFont(nullptr,120));
    canvas->drawTextBlob(blob.get(),0,160,paint);
  });

}