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

int main() {
  constexpr int kFontSize = 40;
//  FreeTypeLib freeTypeLib;
//  auto face = freeTypeLib.LoadFace("assets/fonts/Inter-Regular.otf",
//                                   kFontSize * 64, 0, 0);
  auto data = SkData::MakeFromFileName("assets/fonts/Inter-Regular.otf");
  assert(data);

  auto skia_typeface = SkTypeface::MakeFromStream(new SkMemoryStream(data),0);
  auto font = HBFontPtr(hb_ft_font_create(face.get(), nullptr));

  assert(face);
  HBBufferPtr buf(hb_buffer_create());
  hb_buffer_set_unicode_funcs(buf.get(), hb_icu_get_unicode_funcs());
  hb_buffer_set_direction(buf.get(), HB_DIRECTION_LTR);
  hb_buffer_set_script(buf.get(), HB_SCRIPT_LATIN);
  hb_buffer_set_language(buf.get(), hb_language_from_string("en", -1));

  // layout the text
  hb_buffer_add_utf8(buf.get(), u8"Hello,World!", -1, 0, -1);
  hb_shape(font.get(), buf.get(), nullptr, 0);

  // hand the layout to skia to render
  unsigned int glyph_count;
  hb_glyph_info_t* glyph_info =
      hb_buffer_get_glyph_infos(buf.get(), &glyph_count);
  hb_glyph_position_t* glyph_pos =
      hb_buffer_get_glyph_positions(buf.get(), &glyph_count);

  SkTextBlobBuilder text_blob_builder;
  Run([](auto canvas) {});
}