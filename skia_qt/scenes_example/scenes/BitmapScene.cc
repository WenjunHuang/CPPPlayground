//
// Created by rick on 2021/7/30.
//

#include "BitmapScene.h"
#include <skia/core/SkBitmap.h>
#include <skia/core/SkImage.h>
#include <skia/core/SkImageFilter.h>
#include <skia/core/SkMatrix.h>
#include <skia/core/SkPixelRef.h>
#include <skia/core/SkShader.h>
#include <skia/effects/SkImageFilters.h>
#include <QDebug>
#include <algorithm>

BitmapScene::BitmapScene() : _shapes{100} {
  for (int i = 0; i < 100; i++) {
    SkPath path;
    switch (_random.bounded(4)) {
      case 0:
        path.addRect(SkRect::MakeXYWH(-0.5f, 0.5f, 1.0f, 1.0f));
        break;
      case 1:
        path.addCircle(0, 0, 0.5f);
        break;
      case 2:
        path.moveTo(0, -0.5f).lineTo(0.5f, 0.36f).lineTo(-0.5f, 0.36f).close();
        break;
      case 3:
        path.addRRect(SkRRect::MakeRectXY(
            SkRect::MakeXYWH(-0.6f, -0.4f, 1.2f, 0.8f), 0.4f, 0.4f));
        break;
    }
    path.transform(SkMatrix().setRotate(_random.bounded(360.0)));
    path.transform(SkMatrix::Scale(10.0 + _random.bounded(250.0),
                                   10.0 + _random.bounded(250.0)));
    path.transform(
        SkMatrix::Translate(_random.bounded(1920), _random.bounded(1080)));

    _shapes.append(std::make_pair(std::move(path),
                                  0xFF000000 | _random.bounded(0xFFFFFF)));
  }
}

SkRect intersectSkRects(const SkRect& src, const SkRect& target) {
  if (!src.intersects(target))
    return SkRect::MakeEmpty();
  else
    return SkRect::MakeLTRB(std::max(src.left(), target.left()),
                            std::max(src.top(), target.top()),
                            std::min(src.right(), target.right()),
                            std::min(src.bottom(), target.bottom()));
}

void drawFrame(SkCanvas* canvas, SkRect frame) {
  SkPaint stroke;
  stroke.setAntiAlias(true);
  stroke.setStyle(SkPaint::kStroke_Style);
  stroke.setColor(0xFFE5E5E5);
  stroke.setStrokeWidth(1);
  canvas->drawRect(frame, stroke);
}

void drawPixels(SkCanvas* canvas, SkIRect target, SkIRect screen, float scale) {
  if (!target.intersect(screen))
    return;

  auto inner = SkIRect::MakeXYWH(target.fLeft + 10, target.fTop + 10,
                                 target.width() - 20, target.height() - 20);
  if (inner.isEmpty())
    return;

  SkBitmap bitmap;
  auto srcInfo =
      SkImageInfo::MakeS32(target.width() * scale, target.height() * scale,
                           SkAlphaType::kOpaque_SkAlphaType);
  bitmap.allocPixelsFlags(srcInfo, SkBitmap::kZeroPixels_AllocFlag);

  if (canvas->readPixels(bitmap, target.fLeft * scale, target.fTop * scale)) {
    auto dstInfo =
        SkImageInfo::Make(inner.width() * scale, inner.height() * scale,
                          SkColorType::kARGB_4444_SkColorType,
                          SkAlphaType::kUnpremul_SkAlphaType);
    auto rowBytes = dstInfo.minRowBytes();
    QByteArray byteArray(dstInfo.height() * rowBytes, '\0');
    if (bitmap.readPixels(dstInfo, byteArray.data(), rowBytes, scale * 10,
                          scale * 10)) {
      auto bpp = dstInfo.bytesPerPixel();
      for (int i = 0; i < byteArray.size() / bpp / 2; i++) {
        for (int j = 0; j < bpp; j++) {
          auto i1 = i * bpp + j;
          auto i2 = (byteArray.size() / bpp - i - 1) * bpp + j;

          char b = byteArray[i1];
          byteArray[i1] = byteArray[i2];
          byteArray[i2] = b;
        }
      }
      bitmap.installPixels(dstInfo, byteArray.data(), rowBytes);
      bitmap.notifyPixelsChanged();
      bitmap.setImmutable();
      auto image = SkImage::MakeFromBitmap(bitmap);
      canvas->drawImageRect(image,
                            SkRect::MakeWH(image->width(), image->height()),
                            SkRect::Make(inner), SkSamplingOptions(), nullptr,
                            SkCanvas::kStrict_SrcRectConstraint);
    }
  }
  drawFrame(canvas, SkRect::Make(target));
}

void drawBitmapCanvas(SkCanvas* canvas, SkIRect target, float scale) {
  SkBitmap bitmap;
  SkPath path;
  SkPaint stroke;
  stroke.setAntiAlias(true);
  stroke.setColor(0xFFE76F51);
  stroke.setStyle(SkPaint::kStroke_Style);
  stroke.setStrokeWidth(10);

  bitmap.allocPixels(SkImageInfo::MakeS32(target.width() * scale,
                                          target.height() * scale,
                                          SkAlphaType::kPremul_SkAlphaType));
  bitmap.eraseColor(0x80A8DADC);

  auto color = bitmap.getColor(10, 10);
  auto alpha = bitmap.getAlphaf(10, 10);

  bitmap.erase(0x00FFFFFF, SkIRect::MakeXYWH((target.width() / 2 - 10) * scale,
                                             (target.height() / 2 - 10) * scale,
                                             20 * scale, 20 * scale));
  color =
      bitmap.getColor(target.width() / 2 * scale, target.height() / 2 * scale);
  alpha =
      bitmap.getAlphaf(target.width() / 2 * scale, target.height() / 2 * scale);
  SkCanvas canvas2{bitmap};
  canvas2.scale(scale, scale);
  path.moveTo(0, target.height() / 2)
      .lineTo(target.width() / 2, target.height() / 2 - target.width() / 2)
      .lineTo(target.width(), target.height() / 2)
      .lineTo(target.width() / 2, target.height() / 2 + target.width() / 2)
      .close();
  canvas2.drawPath(path, stroke);

  bitmap.setImmutable();
  auto image = SkImage::MakeFromBitmap(bitmap);
  canvas->drawImageRect(image, SkRect::Make(target), SkSamplingOptions());

  drawFrame(canvas, SkRect::Make(target));
}

void drawGray(SkCanvas* canvas, SkIRect target, float scale) {
  SkBitmap bitmap;
  bitmap.allocPixels(SkImageInfo::Make(
      target.width() * scale, target.height() * scale,
      SkColorType::kGray_8_SkColorType, SkAlphaType::kOpaque_SkAlphaType));
  if (canvas->readPixels(bitmap, (target.left() - 10) * scale,
                         (target.top() - 10) * scale)) {
    bitmap.setImmutable();
    canvas->drawImageRect(SkImage::MakeFromBitmap(bitmap), SkRect::Make(target),
                          SkSamplingOptions());
  }

  drawFrame(canvas, SkRect::Make(target));
}

static void drawBlur(SkCanvas* canvas,
                     SkIRect target,
                     SkIRect screen,
                     int radius,
                     float scale) {
  if (!target.intersect(screen))
    return;
  auto radiusScale = radius * scale;
  auto extended = SkRect::MakeLTRB(
      target.left() - radiusScale, target.top() - radiusScale,
      target.right() + radiusScale, target.bottom() + radiusScale);

  SkBitmap bitmap;
  //  bitmap.allocPixels(SkImageInfo::Make(
  //      extended.width() * scale,
  //      extended.height() * scale,
  //      SkColorType::kGray_8_SkColorType, SkAlphaType::kOpaque_SkAlphaType));
  bitmap.allocPixels(SkImageInfo::MakeS32(extended.width() * scale,
                                          extended.height() * scale,
                                          SkAlphaType::kOpaque_SkAlphaType));
  if (canvas->readPixels(bitmap, extended.left() * scale,
                         extended.top() * scale)) {
    auto mat = SkMatrix::Scale(1.0f / scale, 1.0f / scale);
    auto shader = bitmap.makeShader(SkSamplingOptions(), &mat);
    auto blur =
        SkImageFilters::Blur(radius, radius, SkTileMode::kClamp, nullptr);

    SkPaint fill;
    fill.setAntiAlias(true);
    fill.setShader(shader);
    fill.setImageFilter(blur);

    canvas->save();
    canvas->translate(extended.left(), extended.top());
    SkRect targetRelative = SkRect::Make(target);
    targetRelative.offset(-extended.left(), -extended.top());
    canvas->clipRect(targetRelative);
    canvas->drawRect(
        SkRect::MakeXYWH(0, 0, extended.width(), extended.height()), fill);
    canvas->restore();
  }

  drawFrame(canvas, SkRect::Make(target));
}

static void drawSubset(SkCanvas* canvas,
                       SkIRect target,
                       SkIRect screen,
                       float scale) {
  if (!target.intersect(screen))
    return;

  SkBitmap src;
  SkBitmap dst;
  SkImageInfo srcInfo = SkImageInfo::MakeS32(
      target.width() * scale, target.height() * scale, kOpaque_SkAlphaType);
  src.allocPixels(srcInfo);
  if (canvas->readPixels(src, target.width() * scale,
                         target.height() * scale)) {
    src.extractSubset(&dst, SkIRect::MakeXYWH(target.width() / 4 * scale,
                                              target.height() / 4 * scale,
                                              target.width() / 2 * scale,
                                              target.height() / 2 * scale));
    dst.setImmutable();
    auto image = dst.asImage();
    canvas->drawImageRect(image, SkRect::Make(target), SkSamplingOptions(),
                          nullptr);
  }
  drawFrame(canvas, SkRect::Make(target));
}

static void drawPixelRef(SkCanvas* canvas,
                         SkIRect target,
                         SkIRect screen,
                         float scale) {
  if (!target.intersect(screen))
    return;

  SkBitmap src, dst;
  auto info = SkImageInfo::MakeS32(
      target.width() * scale, target.height() * scale, kOpaque_SkAlphaType);
  src.allocPixels(info);

  if (canvas->readPixels(src, target.fLeft * scale, target.fTop * scale)) {
    auto pixelRef = src.pixelRef();
    dst.setInfo(
        info.makeWH(target.width() / 2 * scale, target.height() / 2 * scale));
    dst.setPixelRef(sk_ref_sp(pixelRef), target.width() / 4 * scale,
                    target.height() / 4 * scale);
    dst.setImmutable();
    auto image = SkImage::MakeFromBitmap(dst);
    canvas->drawImageRect(image, SkRect::Make(target), SkSamplingOptions(),
                          nullptr);
  }
  drawFrame(canvas, SkRect::Make(target));
}

static void drawAlpha(SkCanvas* canvas,
                      SkIRect target,
                      SkIRect screen,
                      float scale) {
  if (!target.intersect(screen))
    return;
  auto srcInfo = SkImageInfo::MakeS32(
      target.width() * scale, target.height() * scale, kOpaque_SkAlphaType);
  SkBitmap src;
  src.allocPixels(srcInfo);
  size_t len = target.width() * scale;
  for (size_t x = 0; x < len; x++) {
    int alpha = 255.0f * x / len;
    int color = (alpha << 24) | (alpha << 16) | (0 << 8) | (255 - alpha);
    src.erase(color, SkIRect::MakeXYWH(x, 0, 1, target.height() / 2 * scale));
  }

  src.setImmutable();
  auto image = src.asImage();
  canvas->drawImageRect(image,
                        SkRect::MakeXYWH(target.fLeft, target.fTop,
                                         target.width(), target.height() / 2),
                        SkSamplingOptions(), nullptr);
  SkBitmap dst;
  if (src.extractAlpha(&dst)) {
    dst.setImmutable();
    canvas->drawImageRect(
        dst.asImage(),
        SkRect::MakeXYWH(target.fLeft, target.fTop + target.height() / 2,
                         target.width(), target.height() / 2),
        SkSamplingOptions(), nullptr);
  }
  drawFrame(canvas, SkRect::Make(target));
}

void BitmapScene::draw(SkCanvas* canvas,
                       int width,
                       int height,
                       float scale,
                       int xpos,
                       int ypos) {
  SkPaint fill;
  fill.setAntiAlias(true);

  SkPaint stroke;
  stroke.setAntiAlias(true);
  stroke.setColor(0xFFE5E5E5);
  stroke.setStyle(SkPaint::kStroke_Style);
  stroke.setStrokeWidth(1);

  std::for_each(_shapes.begin(), _shapes.end(), [&](const auto& item) {
    fill.setColor(item.second);
    canvas->drawPath(item.first, fill);
  });

  auto screen = SkIRect::MakeXYWH(0, 0, width, height);
  auto bw = 200;
  auto bh = 200;

  auto left = xpos - bw - 20 - bw / 2 - 10;
  auto top = ypos - bh - 20 - bh / 2 - 10;
  drawGray(canvas, SkIRect::MakeXYWH(left, top, bw, bh), scale);

  left = xpos - bw / 2 - 10;
  drawBlur(canvas, SkIRect::MakeXYWH(left, top, bw, bh), screen, 20, scale);

  left = xpos + 10 + bw / 2;
  drawBitmapCanvas(canvas, SkIRect::MakeXYWH(left, top, bw, bh), scale);

  top = ypos - bh / 2 - 10;

  left = xpos - bw - 20 - bw / 2 - 10;
  drawPixels(canvas, SkIRect::MakeXYWH(left, top, bw, bh), screen, scale);

  left = xpos - bw / 2 - 10;
  drawSubset(canvas, SkIRect::MakeXYWH(left, top, bw, bh), screen, scale);

  left = xpos + bw / 2 + 10;
  drawPixelRef(canvas, SkIRect::MakeXYWH(left, top, bw, bh), screen, scale);

  left = xpos - bw - 20 - bw / 2 - 10;
  top = ypos + bh / 2 + 10;
  drawAlpha(canvas, SkIRect::MakeXYWH(left, top, bw, bh), screen, scale);
}
