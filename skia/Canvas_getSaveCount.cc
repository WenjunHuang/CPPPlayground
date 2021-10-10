//
// Created by HUANG WEN JUN on 2021/9/7.
//

#include <skia/core/SkCanvas.h>

int main(){
  SkCanvas simple;
  SkDebugf("depth = %d\n",simple.getSaveCount());
  simple.save();
  SkDebugf("depth = %d\n",simple.getSaveCount());
  simple.restore();
  SkDebugf("depth = %d\n",simple.getSaveCount());

}