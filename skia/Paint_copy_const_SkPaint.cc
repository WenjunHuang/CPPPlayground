//
// Created by HUANG WEN JUN on 2021/9/7.
//

#include <skia/core/Skpaint.h>

int main(){
  SkPaint paint1;
  paint1.setColor(SK_ColorRED);
  SkPaint paint2(paint1);
  paint2.setColor(SK_ColorBLUE);

  SkDebugf("SK_ColorRED %c= paint1.getColor()\n",SK_ColorRED == paint1.getColor()?'=':'!');
  SkDebugf("SK_ColorBLUE %c= paint2.getColor()\n",SK_ColorRED == paint2.getColor()?'=':'!');

}