//
// Created by rick on 2021/9/8.
//
#include "window.h"
int main() {
  Run([](SkCanvas* canvas) {
    canvas->clear(SK_ColorWHITE);
    uint8_t pixels[][12] = {{0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00},
                            {0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00},
                            {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00},
                            {0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF},
                            {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                            {0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00},
                            {0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00},
                            {0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00}};
    SkImageInfo imageInfo = SkImageInfo::MakeA8(8,8);
    SkBitmap bitmap;
    bitmap.installPixels(imageInfo,(void*)pixels,sizeof(pixels[0]));
    SkPaint paint;
    paint.setColor(SK_ColorRED);
    canvas->drawImageRect(SkImage::MakeFromBitmap(bitmap),
                          SkRect::MakeWH(8,8),
                          SkRect::MakeWH(32,32),
                          SkSamplingOptions(),
                          &paint,
                          SkCanvas::kStrict_SrcRectConstraint);

    auto offset = imageInfo.computeOffset(2,3,sizeof(pixels[0]));
    pixels[0][offset] = 0x7F;
    offset = imageInfo.computeOffset(5,3,sizeof(pixels[0]));
    pixels[0][offset] = 0x7F;
    bitmap.installPixels(imageInfo,(void*)pixels,sizeof(pixels[0]));
    canvas->drawImageRect(SkImage::MakeFromBitmap(bitmap),SkRect::MakeWH(8,8),
                          SkRect::MakeWH(128,128),
                          SkSamplingOptions(),
                          &paint,
                          SkCanvas::kStrict_SrcRectConstraint);
  });
}