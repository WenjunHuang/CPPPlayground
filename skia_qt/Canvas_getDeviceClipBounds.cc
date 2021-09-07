//
// Created by HUANG WEN JUN on 2021/9/7.
//
#include <skia/core/SkCanvas.h>
#include <skia/core/SkPath.h>
int main() {
  SkCanvas canvas(256,256);
  SkIRect bounds = canvas.getDeviceClipBounds();
  SkDebugf("left:%d top:%d right:%d bottom:%d\n",
           bounds.fLeft,
           bounds.fTop,
           bounds.fRight,
           bounds.fBottom);
  SkPoint clipPoints[] = {{30,130},{120,130},{120,230}};
  SkPath clipPath;
  clipPath.addPoly(clipPoints, SK_ARRAY_COUNT(clipPoints),true);

  canvas.save();
  canvas.clipPath(clipPath);
  bounds = canvas.getDeviceClipBounds();
  SkDebugf("left:%d top:%d right:%d bottom:%d\n",
           bounds.fLeft,
           bounds.fTop,
           bounds.fRight,
           bounds.fBottom);
  canvas.restore();

  canvas.scale(1.f/2,1.f/2);
  canvas.clipPath(clipPath);
  bounds = canvas.getDeviceClipBounds();
  SkDebugf("left:%d top:%d right:%d bottom:%d\n",
           bounds.fLeft,
           bounds.fTop,
           bounds.fRight,
           bounds.fBottom);


}