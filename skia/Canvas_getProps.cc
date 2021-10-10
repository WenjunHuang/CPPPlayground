//
// Created by HUANG WEN JUN on 2021/9/7.
//
#include <skia/core/SkCanvas.h>
#include <skia/core/SkBitmap.h>

int main(){
  SkBitmap bitmap;
  bitmap.allocN32Pixels(1,1);
  SkCanvas canvas(bitmap,SkSurfaceProps(0,kRGB_V_SkPixelGeometry));
  SkSurfaceProps surfaceProps(0,kUnknown_SkPixelGeometry);
  SkDebugf("isRGB:%d\n",SkPixelGeometryIsRGB(surfaceProps.pixelGeometry()));
  if (!canvas.getProps(&surfaceProps)){
    SkDebugf("getProps failed unexpectedly.\n");
  }
  SkDebugf("isRGB:%d\n", SkPixelGeometryIsRGB(surfaceProps.pixelGeometry()));

}