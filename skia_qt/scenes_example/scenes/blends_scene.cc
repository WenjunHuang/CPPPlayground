//
// Created by rick on 2021/9/11.
//

#include "blends_scene.h"
#include <skia/core/SkTextBlob.h>
void BlendsScene::draw(SkCanvas* canvas,
                       int width,
                       int height,
                       float scale,
                       int xpos,
                       int ypos) {
  SkBlendMode modes[] = {
      SkBlendMode::kClear,      SkBlendMode::kSrc,
      SkBlendMode::kDst,        SkBlendMode::kSrcOver,
      SkBlendMode::kDstOver,    SkBlendMode::kSrcIn,
      SkBlendMode::kDstIn,      SkBlendMode::kSrcOut,
      SkBlendMode::kDstOut,     SkBlendMode::kSrcATop,
      SkBlendMode::kDstATop,    SkBlendMode::kXor,
      SkBlendMode::kPlus,       SkBlendMode::kModulate,
      SkBlendMode::kScreen,     SkBlendMode::kOverlay,
      SkBlendMode::kDarken,     SkBlendMode::kLighten,
      SkBlendMode::kColorDodge, SkBlendMode::kColorBurn,
      SkBlendMode::kHardLight,  SkBlendMode::kSoftLight,
      SkBlendMode::kDifference, SkBlendMode::kExclusion,
      SkBlendMode::kMultiply,   SkBlendMode::kHue,
      SkBlendMode::kSaturation, SkBlendMode::kColor,
      SkBlendMode::kLuminosity,
  };
  SkFont font(nullptr, 24);
  canvas->clear(SK_ColorWHITE);
  canvas->translate(20, 20);
  canvas->save();

  SkPaint dst;
  dst.setColor(SK_ColorBLUE);
  SkPaint src;
  src.setColor(SK_ColorRED);
  size_t N = sizeof(modes) / sizeof(modes[0]);
  size_t K = (N - 1) / 3 + 1;
  int index = 0;
  for (auto blend_mode : modes) {
    SkAutoCanvasRestore auto_canvas_restore(canvas, true);
    canvas->translate(189.0f * (index / K), 63.0f * (index % K));
    auto desc = SkBlendMode_Name(blend_mode);
    canvas->drawTextBlob(SkTextBlob::MakeFromString(desc, font).get(), 68.0f,
                         30.0f, SkPaint());
    canvas->clipRect(SkRect::MakeWH(63.0f, 63.0f));
    canvas->drawColor(SK_ColorLTGRAY);
    canvas->saveLayer(nullptr, nullptr);
    canvas->clear(SK_ColorTRANSPARENT);
    canvas->drawRect(SkRect::MakeXYWH(0, 0, 42, 42), dst);
    src.setBlendMode(blend_mode);
    canvas->drawRect(SkRect::MakeXYWH(21, 21, 42, 42), src);
    //    canvas->translate(40, 0);
    ++index;
  }
  canvas->restore();
  canvas->translate(0, 40);
}
BlendsScene::BlendsScene() = default;
