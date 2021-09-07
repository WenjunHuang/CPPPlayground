//
// Created by rick on 2021/9/7.
//
#include <skia/core/SkCanvas.h>
static void check_for_rotated_ctm(const SkCanvas* canvas){
  auto matrix = canvas->getLocalToDevice();
  SkDebugf("ctm is identity = %s\n",matrix == SkM44()?"true":"false");
}
int main(){
  SkCanvas canvas;
  check_for_rotated_ctm(&canvas);
  canvas.rotate(30);
  check_for_rotated_ctm(&canvas);
}