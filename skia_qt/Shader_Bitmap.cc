//
// Created by rick on 2021/9/12.
//
#include "window.h"
#include <skia/core/SkData.h>

int main(){
  auto data = SkData::MakeFromFileName("assets/images/circus.jpg");
  auto image = SkImage::MakeFromEncoded(data);
  Run([&image](auto canvas){
    canvas->clear(SK_ColorWHITE);
    SkMatrix matrix;
    matrix.setScale(0.75f,0.75f);
    matrix.preRotate(30.0f);
    SkPaint paint;
    paint.setShader(image->makeShader(SkTileMode::kRepeat,
                                      SkTileMode::kRepeat,
                                      SkSamplingOptions(),
                                      matrix));
    canvas->drawPaint(paint);
  });
}