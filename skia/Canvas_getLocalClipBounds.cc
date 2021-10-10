//
// Created by HUANG WEN JUN on 2021/9/7.
//

#include <skia/core/SkCanvas.h>
#include <skia/core/SkPath.h>

int main(){
  SkCanvas local(256,256);
  SkCanvas *canvas = &local;
  SkRect bounds = canvas->getLocalClipBounds();
  SkDebugf("left:%g top:%g right:%g bottom:%g\n",
           bounds.fLeft,bounds.fTop,bounds.fRight,bounds.fBottom);
  SkPoint clipPoints[] = {{30,130},{120,130},{120,230}};
  SkPath clipPath;
  clipPath.addPoly(clipPoints, SK_ARRAY_COUNT(clipPoints),true);
  canvas->clipPath(clipPath);
  bounds = canvas->getLocalClipBounds();
  SkDebugf("left:%g top:%g right:%g bottom:%g\n",
           bounds.fLeft,bounds.fTop,bounds.fRight,bounds.fBottom);

  canvas->scale(2,2);
  bounds = canvas->getLocalClipBounds();
  SkDebugf("left:%g top:%g right:%g bottom:%g\n",
           bounds.fLeft,bounds.fTop,bounds.fRight,bounds.fBottom);


}