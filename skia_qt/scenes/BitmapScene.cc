//
// Created by rick on 2021/7/30.
//

#include "BitmapScene.h"
#include <QDebug>
#include <algorithm>
#include <range/v3/range.hpp>
#include <range/v3/view/for_each.hpp>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkImage.h>
#include <skia/core/SkImageFilter.h>
#include <skia/core/SkMatrix.h>
#include <skia/core/SkShader.h>
#include <skia/effects/SkImageFilters.h>

BitmapScene::BitmapScene() : _shapes{100} {
    for (int i = 0; i < 100; i++) {
        SkPath path;
        switch (_random.bounded(4)) {
        case 0: path.addRect(SkRect::MakeXYWH(-0.5f, 0.5f, 1.0f, 1.0f)); break;
        case 1: path.addCircle(0, 0, 0.5f); break;
        case 2:
            path.moveTo(0, -0.5f)
                .lineTo(0.5f, 0.36f)
                .lineTo(-0.5f, 0.36f)
                .close();
            break;
        case 3:
            path.addRRect(SkRRect::MakeRectXY(
                SkRect::MakeXYWH(-0.6f, -0.4f, 1.2f, 0.8f), 0.4f, 0.4f));
            break;
        }
        path.transform(SkMatrix().setRotate(_random.bounded(360.0)));
        path.transform(SkMatrix::MakeScale(10.0 + _random.bounded(250.0)));
        path.transform(
            SkMatrix::MakeTrans(_random.bounded(1920), _random.bounded(1080)));

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

void drawFrame(SkCanvas* canvas,SkRect frame) {
    SkPaint stroke;
    stroke.setStyle(SkPaint::kStroke_Style);
    stroke.setColor(0xFFE5E5E5);
    stroke.setStrokeWidth(1);
    canvas->drawRect(frame, stroke);
}
void drawPixels(SkCanvas* canvas, SkRect target, SkRect screen, float scale) {
    auto intersect = intersectSkRects(target, screen);
    if (intersect.isEmpty())
        return;

    auto     inner = SkRect::MakeXYWH(intersect.left() + 10,
                                      intersect.top() + 10,
                                      intersect.width() - 20,
                                      intersect.height() - 20);
    SkBitmap bitmap;
    auto     srcInfo = SkImageInfo::MakeS32(intersect.width() * scale,
                                            intersect.height() * scale,
                                            SkAlphaType::kOpaque_SkAlphaType);
    bitmap.allocPixelsFlags(srcInfo, SkBitmap::kZeroPixels_AllocFlag);

    if (canvas->readPixels(
            bitmap, intersect.left() * scale, intersect.top() * scale)) {
        auto       dstInfo  = SkImageInfo::Make(intersect.width() * scale,
                                                intersect.height() * scale,
                                                SkColorType::kARGB_4444_SkColorType,
                                                SkAlphaType::kUnpremul_SkAlphaType);
        auto       rowBytes = dstInfo.minRowBytes();
        auto       bpp      = dstInfo.bytesPerPixel();
        QByteArray byteArray(
            std::min<int>(bitmap.height(), dstInfo.height() - scale * 10) *
                rowBytes,
            '\0');
        bitmap.readPixels(
            dstInfo, byteArray.data(), rowBytes, scale * 10, scale * 10);
        for (int i = 0; i < byteArray.size() / bpp / 2; i++) {
            for (int j = 0; j < bpp; j++) {
                auto i1 = i * bpp + j;
                auto i2 = (byteArray.size() / bpp - i - 1) * bpp + j;

                char b        = byteArray[i1];
                byteArray[i1] = byteArray[i2];
                byteArray[i2] = b;
            }
        }
        bitmap.installPixels(dstInfo,byteArray.data(),rowBytes);
        bitmap.notifyPixelsChanged();
        bitmap.setImmutable();
        auto image = SkImage::MakeFromBitmap(bitmap);
        canvas->drawImageRect(image,SkRect::MakeWH(image->width(),image->height()),inner, nullptr);
    }
    drawFrame(canvas,intersect);
}

void drawBitmapCanvas(SkCanvas* canvas, SkRect target, float scale) {
    SkBitmap bitmap;
    SkPath   path;
    SkPaint  stroke;
    stroke.setColor(0xFFE76F51);
    stroke.setStyle(SkPaint::kStroke_Style);
    stroke.setStrokeWidth(10);

    bitmap.allocPixels(SkImageInfo::MakeS32(target.width() * scale,
                                            target.height() * scale,
                                            SkAlphaType::kPremul_SkAlphaType));
    bitmap.eraseColor(0x80A8DADC);

    auto color = bitmap.getColor(10, 10);
    auto alpha = bitmap.getAlphaf(10, 10);

    bitmap.erase(0x00FFFFFF,
                 SkIRect::MakeXYWH((target.width() / 2 - 10) * scale,
                                   (target.height() / 2 - 10) * scale,
                                   20 * scale,
                                   20 * scale));
    color = bitmap.getColor(target.width() / 2 * scale,
                            target.height() / 2 * scale);
    alpha = bitmap.getAlphaf(target.width() / 2 * scale,
                             target.height() / 2 * scale);
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
    canvas->drawImageRect(image,
                          SkRect::MakeWH(image->width(), image->height()),
                          target,
                          nullptr);
    SkPaint stroke2;
    stroke2.setStyle(SkPaint::kStroke_Style);
    stroke2.setColor(0xFFE5E5E5);
    stroke2.setStrokeWidth(1);
    canvas->drawRect(target, stroke2);
}

void drawGray(SkCanvas* canvas, SkRect target, float scale) {
    SkBitmap bitmap;
    bitmap.allocPixels(SkImageInfo::Make(target.width() * scale,
                                         target.height() * scale,
                                         SkColorType::kGray_8_SkColorType,
                                         SkAlphaType::kOpaque_SkAlphaType));
    if (canvas->readPixels(bitmap,
                           (target.left() - 10) * scale,
                           (target.top() - 10) * scale)) {
        bitmap.setImmutable();
        canvas->drawImageRect(SkImage::MakeFromBitmap(bitmap),
                              SkRect::MakeWH(bitmap.width(), bitmap.height()),
                              target,
                              nullptr);
    }

    SkPaint stroke;
    stroke.setColor(0xFFE5E5E5);
    stroke.setStyle(SkPaint::kStroke_Style);
    stroke.setStrokeWidth(1);
    canvas->drawRect(target, stroke);
}

void drawBlur(SkCanvas* canvas, SkRect target, int radius, float scale) {
    //    target = target.intersect(screen);
    SkBitmap bitmap;
    auto     radiusScale = radius * scale;
    auto     extended    = SkRect::MakeLTRB(target.left() - radiusScale,
                                            target.top() - radiusScale,
                                            target.right() + radiusScale,
                                            target.bottom() + radiusScale);
    bitmap.allocPixels(SkImageInfo::MakeS32(qRound(extended.width() * scale),
                                            qRound(extended.height() * scale),
                                            SkAlphaType::kOpaque_SkAlphaType));
    if (canvas->readPixels(bitmap,
                           qRound(extended.left() * scale),
                           qRound(extended.top() * scale))) {
        auto mat    = SkMatrix::MakeScale(1.0f / scale);
        auto shader = bitmap.makeShader(&mat);
        auto blur =
            SkImageFilters::Blur(radius, radius, SkTileMode::kClamp, nullptr);

        SkPaint fill;
        fill.setShader(shader);
        fill.setImageFilter(blur);

        canvas->save();
        canvas->translate(extended.left(), extended.top());
        SkRect targetRelative{target};
        targetRelative.offset(-extended.left(), -extended.top());
        canvas->clipRect(targetRelative);
        canvas->drawRect(
            SkRect::MakeXYWH(0, 0, extended.width(), extended.height()), fill);
        canvas->restore();
    }

    SkPaint stroke;
    stroke.setColor(0xFFE5E5E5);
    stroke.setStyle(SkPaint::kStroke_Style);
    stroke.setStrokeWidth(1);
    canvas->drawRect(target, stroke);
}

void BitmapScene::draw(
    SkCanvas* canvas, int width, int height, float scale, int xpos, int ypos) {
    SkPaint fill;
    SkPaint stroke;
    stroke.setColor(0xFFE5E5E5);
    stroke.setStyle(SkPaint::kStroke_Style);
    stroke.setStrokeWidth(1);

    std::for_each(_shapes.begin(), _shapes.end(), [&](const auto& item) {
        fill.setColor(item.second);
        canvas->drawPath(item.first, fill);
    });

    auto screen = SkRect::MakeXYWH(0, 0, width, height);
    auto bw     = 200;
    auto bh     = 200;

    auto left = xpos - bw - 20 - bw / 2 - 10;
    auto top  = ypos - bh - 20 - bh / 2 - 10;
    drawGray(canvas, SkRect::MakeXYWH(left, top, bw, bh), scale);

    left = xpos - bw / 2 - 10;
    drawBlur(canvas, SkRect::MakeXYWH(left, top, bw, bh), 20, scale);

    left = xpos + 10 + bw / 2;
    drawBitmapCanvas(canvas, SkRect::MakeXYWH(left, top, bw, bh), scale);

    left = xpos - bw - 20 - bw / 2 - 10;
    top  = ypos - bh / 2 - 10;
    drawPixels(canvas, SkRect::MakeXYWH(left, top, bw, bh), screen, scale);
}
