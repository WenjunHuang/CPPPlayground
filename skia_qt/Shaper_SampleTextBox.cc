//
// Created by rick on 2021/9/14.
//
#include "window.h"

static const char gText[] =
    "When in the Course of human events it becomes necessary for one people "
    "to dissolve the political bands which have connected them with another "
    "and to assume among the powers of the earth, the separate and equal "
    "station to which the Laws of Nature and of Nature's God entitle them, "
    "a decent respect to the opinions of mankind requires that they should "
    "declare the causes which impel them to the separation.";

void DrawText(SkCanvas* canvas,SkScalar width,SkScalar height,SkColor fg,SkColor bg) {
  SkAutoCanvasRestore acr(canvas, true);

  canvas->clipRect(SkRect::MakeWH(width,height));
  canvas->drawColor(bg);

  SkScalar margin = 20;

  SkPaint paint;
  paint.setColor(fg);

  for (int i = 9;i< 24;i+=2) {
  }
}
int main(){
  Run([](SkCanvas* canvas,int width,int height){
    auto w = width / 3;

  });

}