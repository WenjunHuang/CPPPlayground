//
// Created by HUANG WEN JUN on 2021/9/7.
//
#include <skia/core/SkPaint.h>
#include <skia/effects/SkDashPathEffect.h>

int main(){
  SkPaint paint;
  float intervals[] = {5,5};
  paint.setPathEffect(SkDashPathEffect::Make(intervals, SK_ARRAY_COUNT(intervals),2.5f));
  SkPaint dashed(std::move(paint));
  SkDebugf("path effect unique: %s\n",
           dashed.getPathEffect()->unique()?"true":"false");

}