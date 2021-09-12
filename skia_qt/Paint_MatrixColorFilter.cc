//
// Created by rick on 2021/9/12.
//
#include "window.h"
#include <skia/core/SkColorFilter.h>
void f(SkCanvas* canvas,SkScalar x,SkScalar y,SkScalar *color_matrix,const sk_sp<SkImage>& image) {
  SkPaint paint;
  paint.setColorFilter(SkColorFilters::Matrix(color_matrix));
  canvas->drawImage(image,x,y,SkSamplingOptions(),&paint);
}

int main(){
  auto data = SkData::MakeFromFileName("assets/images/circus.jpg");
  auto image = SkImage::MakeFromEncoded(data);
  Run([&image](auto canvas){
    canvas->scale(0.25,0.25);
    SkScalar color_matrix1[] = {
        0, 1, 0, 0, 0,
        0, 0, 1, 0, 0,
        1, 0, 0, 0, 0,
        0, 0, 0, 1, 0};
    f(canvas,0,0,color_matrix1,image);

    SkScalar grayscale[20] = {
        0.21f, 0.72f, 0.07f, 0.0f, 0.0f,
        0.21f, 0.72f, 0.07f, 0.0f, 0.0f,
        0.21f, 0.72f, 0.07f, 0.0f, 0.0f,
        0.0f,  0.0f,  0.0f,  1.0f, 0.0f};
    f(canvas,0,512,grayscale,image);
  });
}
