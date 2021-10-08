//
// Created by rick on 2021/9/8.
//

#include <skia/core/SkImageInfo.h>
#include "window.h"
int main() {
  Run([](auto canvas) {
    canvas->clear(SK_ColorWHITE);

    SkImageInfo info = SkImageInfo::MakeN32Premul(2, 2);

    auto minRowBytes = info.minRowBytes();
    auto size = info.computeByteSize(100000);

    SkAutoTMalloc<SkPMColor> storage(size);
    SkPMColor* pixels = storage.get();
    SkBitmap bitmap;
    bitmap.setInfo(info);
    bitmap.setPixels(pixels);
    bitmap.eraseColor(SK_ColorRED);
    canvas->scale(50, 50);
    canvas->rotate(8);
    canvas->drawImage(SkImage::MakeFromBitmap(bitmap), 2, 0);
  });
}