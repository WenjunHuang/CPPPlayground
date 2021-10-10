//
// Created by rick on 2021/9/7.
//
#include <skia/core/SkCanvas.h>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkFont.h>

int main(){
  SkBitmap bitmap;
  // create a bitmap 5 wide and 11 high
  bitmap.allocPixels(SkImageInfo::MakeN32Premul(5,11));
  SkCanvas canvas(bitmap);
  canvas.clear(SK_ColorWHITE); // white is Unpremultiplied, in ARGB order
  SkPixmap pixmap;
  if (!canvas.peekPixels(&pixmap))
    SkDebugf("peekPixels should never fail.\n");

  auto pixels = pixmap.addr32(); // pointers to top-left of bitmap
  SkPMColor pmWhite = pixels[0];
  SkPaint paint;
  SkFont font;
  canvas.drawString("!",1,10,font,paint);
  for(int y =0;y<bitmap.height();++y){
    for(int x = 0;x<bitmap.width();++x){
      SkDebugf("%c",*pixels++ == pmWhite?'-':'x');
    }
    SkDebugf("\n");
  }

}