//
// Created by HUANG WEN JUN on 2021/9/22.
//
#include <fmt/printf.h>
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
  constexpr int kFontSizePt = 16;  // point 1/72 inch
  constexpr int kFontSizePx = kFontSizePt * 72 / 72;
  //  FreeTypeLib freeTypeLib;
  //  auto face = freeTypeLib.LoadFace("assets/fonts/Inter-Regular.otf",
  //                                   kFontSize * 64, 0, 0);
  auto data = SkData::MakeFromFileName("../assets/fonts/amiri-regular.ttf");
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
  hb_font_set_ptem(font.get(), kFontSizePt);
  auto upem = hb_face_get_upem(face.get());
  auto ppu = (float)kFontSizePx / upem;

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
  const char arb[] = u8"تسجّل يتكلّم";
  hb_buffer_add_utf8(buf.get(), arb, -1, 0, -1);
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
  paint_font.setSize(kFontSizePt);

  auto run_buffer = text_blob_builder.allocRunPos(paint_font, glyph_count);
  double x = 0;
  double y = 0;
  std::vector<SkPoint> points;
  int text_bound_width = 0;
  int text_bound_height = 0;
  for (int i = 0; i < glyph_count; i++) {
    auto& gi = glyph_info[i];
    auto& gp = glyph_pos[i];
    auto px = x + glyph_pos[i].x_offset * ppu;
    auto py = y - glyph_pos[i].y_offset * ppu;

    run_buffer.glyphs[i] = glyph_info[i].codepoint;
    run_buffer.points()[i] = SkPoint::Make(x, y);
    x += glyph_pos[i].x_advance * ppu;
    y += glyph_pos[i].y_advance * ppu;
    points.push_back(SkPoint::Make(px, py));

    text_bound_width += glyph_pos[i].x_advance * ppu;
    text_bound_height += glyph_pos[i].y_advance * ppu;
  }
  auto text_blob = text_blob_builder.make();

  Run([&](auto canvas, int width, int height) {
    //    fmt::print("width:{},height:{}\n",width,height);
    SkPaint borderPaint;
    borderPaint.setStyle(SkPaint::kStroke_Style);
    borderPaint.setStrokeWidth(1);
    borderPaint.setColor(SK_ColorBLACK);

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(SK_ColorBLACK);
    canvas->clear(SK_ColorWHITE);
    canvas->drawTextBlob(text_blob, width / 2, height / 2, paint);

    auto bounds = SkRect::MakeWH(text_bound_width, text_bound_height);
    canvas->drawRect(bounds.makeOffset(width / 2, height / 2), borderPaint);
    auto point = SkPoint::Make(width / 2, height / 2);

    paint.setStrokeCap(SkPaint::kRound_Cap);
    paint.setColor(SK_ColorRED);
    paint.setStrokeWidth(5);
    canvas->drawLine(point, point, paint);

    for (auto& p : points) {
      p.offset(width / 2, height / 2);
      SkPoint dp = SkPoint::Make(p.x() + width / 2, p.y() + height / 2);
      canvas->drawLine(dp, dp, paint);
    }
  });
}