//
// Created by rick on 2021/9/7.
//

#include <skia/core/SkBitmap.h>
#include <skia/core/SkCanvas.h>

int main(){
  SkBitmap bitmap;
  bitmap.allocPixels(SkImageInfo::MakeN32Premul(20,30));
  SkCanvas canvas(bitmap,SkSurfaceProps(0,kUnknown_SkPixelGeometry));
  canvas.clipRect(SkRect::MakeWH(10,40));
  SkIRect clipDeviceBounds = canvas.getDeviceClipBounds();
  if (clipDeviceBounds.isEmpty()){
    SkDebugf("Empty clip bounds is unexpected!\n");
  }
  SkDebugf("clip=%d,%d\n",clipDeviceBounds.width(),clipDeviceBounds.height());
  SkISize baseLayerSize = canvas.getBaseLayerSize();
  SkDebugf("size=%d,%d\n",baseLayerSize.width(),baseLayerSize.height());
}