//
// Created by rick on 2021/9/8.
//

#include "window.h"
int main() {
  Run([](auto canvas) {
    constexpr size_t width = 256;
    constexpr size_t height = 32;

    SkImageInfo info = SkImageInfo::MakeN32Premul(width, height);
    SkColor gradColors[] = {0xFFAA0055, 0xFF11CC88};
    SkPoint gradPoints[] = {{0, 0}, {width, 0}};
    SkPaint gradPaint;
    gradPaint.setShader(SkGradientShader::MakeLinear(
        gradPoints, gradColors, nullptr, SK_ARRAY_COUNT(gradColors),
        SkTileMode::kClamp));

    SkBitmap bitmap;
    bitmap.allocPixels(SkImageInfo::MakeN32(width,height,kPremul_SkAlphaType));

    SkCanvas offscreen(bitmap);
    offscreen.drawRect(SkRect::MakeWH(width,height),gradPaint);
    canvas->drawImage(bitmap.asImage(),0,0);

    bitmap.allocPixels(SkImageInfo::MakeS32(width,height,kPremul_SkAlphaType));
    SkCanvas sRGBOffscreen(bitmap);
    sRGBOffscreen.drawRect(SkRect::MakeWH(width,height),gradPaint);
    canvas->drawImage(bitmap.asImage(),0,48);

    SkBitmap noColorSpaceBitmap;
    noColorSpaceBitmap.setInfo(SkImageInfo::MakeN32(width,height,kPremul_SkAlphaType));
    noColorSpaceBitmap.setPixels(bitmap.getAddr(0,0));
    canvas->drawImage(noColorSpaceBitmap.asImage(),0,96);
  });
}