//
// Created by rick on 2021/9/12.
//
#include <skia/effects/SkTableColorFilter.h>
#include "window.h"

int main() {
  auto data = SkData::MakeFromFileName("assets/images/circus.jpg");
  auto image = SkImage::MakeFromEncoded(data);

  Run([&image](auto canvas) {
    canvas->scale(0.5, 0.5);
    uint8_t ct[256];
    for (int i = 0; i < 256; ++i) {
      int x = (i - 96) * 255 / 64;
      ct[i] = x < 0 ? 0 : x > 255 ? 255 : x;
    }
    SkPaint paint;
    paint.setColorFilter(SkTableColorFilter::MakeARGB(nullptr, ct, ct, ct));
    canvas->drawImage(image, 0, 0, SkSamplingOptions(), &paint);
  });
}
