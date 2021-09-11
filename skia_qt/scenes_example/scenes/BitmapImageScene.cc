//
// Created by HUANG WEN JUN on 2021/9/9.
//

#include "BitmapImageScene.h"
#include <skia/core/SkBitmap.h>
#include <skia/core/SkColor.h>
#include <skia/core/SkImage.h>
#include <cmath>
#include <execution>

struct BitmapImageScenePrivate {
  sk_sp<SkImage> image;
  int x, y;

  void Advance(SkCanvas* canvas, int width) {
    canvas->restore();
    x += 220;
    if (x + 220 >= width) {
      x = 20;
      y += 240;
    }
    canvas->save();
    canvas->translate(x, y);
  }
};

float phase() {
  using namespace std::chrono;
  auto angle =
      (duration_cast<milliseconds>(system_clock::now().time_since_epoch())
           .count() %
       5000) /
      5000.0 * M_PI * 2;
  auto phase = std::sin(angle) * 1.2;
  phase = std::min(1.0, std::max(-1.0, phase));
  return static_cast<float>((phase + 1.0) / 2.0);
}

float luminocity(SkColorType colorType, uint32_t color) {
  return SkColorGetR(color) + SkColorGetG(color) + SkColorGetB(color);
}

void PixelSorting(SkCanvas* canvas, uint8_t* pixels, const SkImageInfo& info) {
  auto threshold = 100 + phase() * 100;
  auto colorType = info.colorType();

  for (size_t x = 0; x < info.width(); ++x) {
    // read pixels
    std::vector<uint32_t> column(info.height());
    for (int y = 0; y < info.height(); ++y) {
      auto idx = (y * info.width() + x) * info.bytesPerPixel();
      column[y] = *(uint32_t*)(pixels + idx);  // pixels.getInt(idx);
    }

    // sort pixels
    int last_idx = 0;
    auto cmp = [=](int32_t a, int32_t b) {
      return luminocity(colorType, a) > luminocity(colorType, b);
    };
    for (int y = 0; y < info.height() - 1; ++y) {
      if (std::abs(luminocity(colorType, column[y]) -
                   luminocity(colorType, column[y + 1])) > threshold) {
        std::sort(std::execution::par, column.begin() + last_idx,
                  column.begin() + y, cmp);
        last_idx = y;
      }
    }
    std::sort(std::execution::par, column.begin() + last_idx, column.end(),
              cmp);

    // write pixels
    for (int y = 0; y < info.height(); ++y) {
      *(int32_t*)(pixels + (y * info.width() + x) * info.bytesPerPixel()) =
          column[y];
    }
  }
}
BitmapImageScene::BitmapImageScene() {
  dptr_ = std::make_unique<BitmapImageScenePrivate>();
  dptr_->image =
      SkImage::MakeFromEncoded(resourceAsData("/assets/images/IMG_7098.jpeg"));
  dptr_->x = 0;
  dptr_->y = 0;
}

BitmapImageScene::~BitmapImageScene() = default;

void BitmapImageScene::draw(SkCanvas* canvas,
                            int width,
                            int height,
                            float scale,
                            int xpos,
                            int ypos) {
  canvas->clear(SK_ColorWHITE);
  canvas->save();
  canvas->translate(20, 20);
  dptr_->x = 20;
  dptr_->y = 20;

  // Image
  canvas->drawImageRect(dptr_->image, SkRect::MakeXYWH(0, 0, 200, 200),
                        SkSamplingOptions(), nullptr);
  canvas->drawString("Image", 0, 220, inter13, blackFill);
  dptr_->Advance(canvas, width);

  // Bitmap + Image.readPixels
  {
    SkBitmap bitmap;
    bitmap.allocPixels(SkImageInfo::MakeS32(400, 400, kOpaque_SkAlphaType));
    dptr_->image->readPixels(nullptr, bitmap.pixmap(), 0, 0,
                             SkImage::kDisallow_CachingHint);
    bitmap.setImmutable();
    canvas->drawImageRect(bitmap.asImage(), SkRect::MakeXYWH(0, 0, 200, 200),
                          SkSamplingOptions(), nullptr);
    canvas->drawString("Image.readPixels", 0, 220, inter13, blackFill);
    dptr_->Advance(canvas, width);
  }

  // Bitmap + Image.readPixels(50,50)
  {
    SkBitmap bitmap;
    bitmap.allocPixels(
        SkImageInfo::Make(300, 300, kGray_8_SkColorType, kOpaque_SkAlphaType));
    dptr_->image->readPixels(nullptr, bitmap.pixmap(), 50, 50,
                             SkImage::kDisallow_CachingHint);
    bitmap.setImmutable();
    canvas->drawImageRect(bitmap.asImage(), SkRect::MakeXYWH(25, 25, 150, 150),
                          SkSamplingOptions(), nullptr);
    canvas->drawString("Image.readPixels(50,50)", 0, 220, inter13, blackFill);
    dptr_->Advance(canvas, width);
  }

  SkImageInfo info;
  sk_sp<SkData> pixels;
  //  std::unique_ptr<uint8_t[]> pixels ;
  {
    // Bitmap readPixels/installPixels
    SkBitmap bitmap;
    bitmap.allocPixels(dptr_->image->imageInfo());
    dptr_->image->readPixels(nullptr, bitmap.pixmap(), 0, 0,
                             SkImage::kDisallow_CachingHint);
    info = bitmap.info();
    size_t byte_length = bitmap.rowBytes() * (info.height() + 1);
    //    pixels = std::make_unique<uint8_t[]>(byte_length);
    pixels =
        SkData::MakeFromMalloc(sk_malloc_canfail(byte_length), byte_length);
    bitmap.readPixels(info, pixels->writable_data(), bitmap.rowBytes(), 0, 0);
    PixelSorting(canvas, (uint8_t*)pixels->writable_data(), info);
    bitmap.installPixels(info, pixels->writable_data(), bitmap.rowBytes());
    bitmap.setImmutable();
    canvas->drawImageRect(bitmap.asImage(), SkRect::MakeXYWH(0, 0, 200, 200),
                          SkSamplingOptions(), nullptr);
    canvas->drawString("Bitmap.readPixels/installPixels", 0, 220, inter13,
                       blackFill);
    dptr_->Advance(canvas, width);
  }

  {
    // Bitmap peekPixels
    SkBitmap bitmap;
    bitmap.allocPixels(dptr_->image->imageInfo());
    dptr_->image->readPixels(nullptr, bitmap.pixmap(), 0, 0);
    SkPixmap pixmap;
    bitmap.peekPixels(&pixmap);
    PixelSorting(canvas, (uint8_t*)pixmap.writable_addr(0, 0), bitmap.info());

    bitmap.setImmutable();
    auto image = bitmap.asImage();
    canvas->drawImageRect(image, SkRect::MakeXYWH(0, 0, 200, 200),
                          SkSamplingOptions(), nullptr);
    canvas->drawString("Bitmap.peekPixels", 0, 220, inter13, blackFill);
    dptr_->Advance(canvas, width);
  }

  {
    // Image.makeRaster
    auto imageFromPixel =
        SkImage::MakeRasterData(info, pixels, info.minRowBytes());
    canvas->drawImageRect(imageFromPixel, SkRect::MakeXYWH(0, 0, 200, 200),
                          SkSamplingOptions(), nullptr);
    canvas->drawString("Image.makeRaster", 0, 220, inter13, blackFill);
    dptr_->Advance(canvas, width);
  }
}
