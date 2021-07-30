//
// Created by rick on 2021/7/30.
//

#include "BitmapScene.h"
#include <range/v3/range.hpp>
#include <range/v3/view/for_each.hpp>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkImage.h>
#include <skia/core/SkMatrix.h>

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
    auto left   = xpos - bw - 20 - bw / 2 - 10;
    auto top    = ypos - bh - 20 - bh / 2 - 10;
    drawGray(canvas, SkRect::MakeXYWH(left, top, bw, bh), scale);
}
