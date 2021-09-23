//
// Created by HUANG WEN JUN on 2021/9/22.
//
#include <harfbuzz/hb-ft.h>
#include <harfbuzz/hb-icu.h>
#include <harfbuzz/hb.h>
#include <skia/core/SkTextBlob.h>
#include <cassert>
#include <memory>
#include "free_type_lib.h"
#include "window.h"
using HBBufferPtr =
    std::unique_ptr<hb_buffer_t, FunctionWrapper<hb_buffer_destroy>>;
using HBFontPtr = std::unique_ptr<hb_font_t, FunctionWrapper<hb_font_destroy>>;
using HBBlobPtr = std::unique_ptr<hb_blob_t, FunctionWrapper<hb_blob_destroy>>;
using HBFacePtr = std::unique_ptr<hb_face_t, FunctionWrapper<hb_face_destroy>>;

int main() {
  constexpr int kFontSize = 40;
  //  FreeTypeLib freeTypeLib;
  //  auto face = freeTypeLib.LoadFace("assets/fonts/Inter-Regular.otf",
  //                                   kFontSize * 64, 0, 0);
  auto data =
      SkData::MakeFromFileName("assets/fonts/amiri-regular.ttf");
  assert(data != nullptr);

  auto skia_typeface =
      SkTypeface::MakeFromStream(std::make_unique<SkMemoryStream>(data), 0);
  assert(skia_typeface != nullptr);

  auto destroy = [](void* d) { static_cast<SkData*>(d)->unref(); };
  data->ref();
  HBBlobPtr blob(hb_blob_create((const char*)data->data(),
                                (unsigned int)data->size(),
                                HB_MEMORY_MODE_READONLY, data.get(), destroy));
  assert(blob);
  hb_blob_make_immutable(blob.get());

  HBFacePtr face(hb_face_create(blob.get(), 0));
  HBFontPtr font(hb_font_create(face.get()));

  //  - create buffer
  //  - add text to buffer    ---> buffer contains Unicode text now
  //  - call hb_shape() on it
  //  - use output glyphs     ---> buffer contains positioned glyphs now
  HBBufferPtr buf(hb_buffer_create());
  hb_buffer_set_unicode_funcs(buf.get(), hb_icu_get_unicode_funcs());
  hb_buffer_set_direction(buf.get(), HB_DIRECTION_RTL);
  hb_buffer_set_script(buf.get(), HB_SCRIPT_ARABIC);
  hb_buffer_set_language(buf.get(), hb_language_from_string("ar", -1));

  // layout the text
  hb_buffer_add_utf8(buf.get(), u8"تسجّل يتكلّم", -1, 0, -1);
  hb_shape(font.get(), buf.get(), nullptr, 0);

  // hand the layout to skia to render
  unsigned int glyph_count;
  hb_glyph_info_t* glyph_info =
      hb_buffer_get_glyph_infos(buf.get(), &glyph_count);
  hb_glyph_position_t* glyph_pos =
      hb_buffer_get_glyph_positions(buf.get(), &glyph_count);

  SkTextBlobBuilder text_blob_builder;
  SkFont paint_font;
  paint_font.setTypeface(skia_typeface);
  paint_font.setSubpixel(true);
  paint_font.setSize(kFontSize);

  auto run_buffer = text_blob_builder.allocRunPos(paint_font, glyph_count);
  double x = 0;
  double y = 0;
  constexpr float kFontSizeScale = 64.0;
  for (int i = 0; i < glyph_count; i++) {
    auto& gi = glyph_info[i];
    auto& gp = glyph_pos[i];
    run_buffer.glyphs[i] = glyph_info[i].codepoint;
    run_buffer.points()[i] =
        SkPoint::Make(x + glyph_pos[i].x_offset / kFontSizeScale,
                      y - glyph_pos[i].y_offset / kFontSizeScale);
    x += glyph_pos[i].x_advance / kFontSize;
    y += glyph_pos[i].y_advance / kFontSize;
  }
  auto text_blob = text_blob_builder.make();

  Run([&](auto canvas, int width, int height) {
    SkPaint borderPaint;
    borderPaint.setStyle(SkPaint::kStroke_Style);
    borderPaint.setStrokeWidth(1);
    borderPaint.setColor(SK_ColorBLUE);
    auto bound = text_blob->bounds();
    auto rect = SkRect::MakeWH(bound.width(),bound.height());
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(SK_ColorBLUE);
    canvas->clear(SK_ColorWHITE);
    canvas->drawTextBlob(text_blob, width / 2,height / 2, paint);

    canvas->drawRect(rect, borderPaint);


  });
}