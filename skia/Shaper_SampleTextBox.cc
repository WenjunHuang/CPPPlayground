//
// Created by rick on 2021/9/14.
//
#include <skia/modules/shaper/SkShaper.h>
#include <memory>
#include "window.h"

static const char gText[] =
    "When 🐉☺️❤️👮🏿👨‍👩‍👧‍👦🚵 in the Course of human events it "
    "becomes necessary for one people "
    "Lettre à ma meilleure amie La célébration de Pâques "
    "to dissolve the political bands which have connected them with another "
    "吃葡萄不吐葡萄皮，落日"
    "station to which the Laws of Nature and of Nature's God entitle them, "
    "صباح الخير"
    "おはようございます";

auto shaper = SkShaper::Make();
void DrawText(SkCanvas* canvas,
              SkScalar width,
              SkScalar height,
              SkColor fg,
              SkColor bg) {
  SkAutoCanvasRestore acr(canvas, true);

  canvas->clipRect(SkRect::MakeWH(width, height));
  canvas->drawColor(bg);

  SkScalar margin = 20;

  SkPaint paint;
  paint.setColor(fg);

  const char* utf8 = gText;
  size_t utf8_bytes = sizeof(gText) - 1;

  for (int i = 9; i < 24; i += 2) {
    SkShaper::PurgeCaches();
    SkTextBlobBuilderRunHandler builder(gText, {margin, margin});
    SkFont src_font(nullptr, SkIntToScalar(i));
    src_font.setEdging(SkFont::Edging::kSubpixelAntiAlias);
    src_font.setSubpixel(true);

    auto bidi = SkShaper::MakeBiDiRunIterator(utf8, utf8_bytes, 0xfe);
    if (!bidi)
      return;
    auto language = SkShaper::MakeStdLanguageRunIterator(utf8, utf8_bytes);
    if (!language)
      return;

    SkFourByteTag undetermined_script = SkSetFourByteTag('Z', 'y', 'y', 'y');
    auto script =
        SkShaper::MakeScriptRunIterator(utf8, utf8_bytes, undetermined_script);
    if (!script)
      return;

    auto font = SkShaper::MakeFontMgrRunIterator(
        utf8, utf8_bytes, src_font, SkFontMgr::RefDefault(), "Arial",
        SkFontStyle::Bold(), &*language);
    if (!font)
      return;

    shaper->shape(utf8, utf8_bytes, *font, *bidi, *script, *language,
                  width - margin, &builder);
    canvas->drawTextBlob(builder.makeBlob(), 0, 0, paint);
    canvas->translate(0, builder.endPoint().y());
    //    SkFont default_font(nullptr,SkIntToScalar(i));
    //    auto text_blob =
    //    SkTextBlob::MakeFromText(utf8,utf8_bytes,default_font);
    //    canvas->drawTextBlob(text_blob,0,0,paint);
    //    text_blob->
    //        canvas->translate(0,builder.endPoint().y());
  }
}
int main() {
  Run([](SkCanvas* canvas, int width, int height) {
    auto w = width / 3;
    DrawText(canvas, w, height, SK_ColorBLACK, SK_ColorWHITE);
    canvas->translate(w, 0);
    DrawText(canvas, w, height, SK_ColorWHITE, SK_ColorBLACK);
    canvas->translate(w, 0);
    DrawText(canvas, w, height / 2, SK_ColorGRAY, SK_ColorWHITE);
    canvas->translate(0, height / 2);
    DrawText(canvas, w, height / 2, SK_ColorGRAY, SK_ColorBLACK);
  });
}