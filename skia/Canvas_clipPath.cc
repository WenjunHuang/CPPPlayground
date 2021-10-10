//
// Created by rick on 2021/9/7.
//

#include "window.h"
int main(){
  Run([](SkCanvas* canvas){
    canvas->clear(SK_ColorWHITE);

    SkPaint paint;
    paint.setAntiAlias(true);

    SkPath path;
    path.addRect({20,30,100,110});
    path.setFillType(SkPathFillType::kInverseWinding);

    canvas->save();
    canvas->clipPath(path,SkClipOp::kDifference, false);
    canvas->drawCircle(70,100,60,paint);
    canvas->restore();

    canvas->translate(100,100);
    path.setFillType(SkPathFillType::kWinding);
    canvas->clipPath(path,SkClipOp::kIntersect,false);
    canvas->drawCircle(70,100,60,paint);

  },200,200);

}