//
// Created by rick on 2021/9/22.
//

#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <cmath>
#include <iostream>
#include "../util.h"

using namespace std;

struct Glyph {
  Glyph(unsigned char* buffer,
        int width,
        int height,
        float bearing_x,
        float bearing_y)
      : buffer(buffer),
        width(width),
        height(height),
        bearing_x(bearing_x),
        bearing_y(bearing_y) {}

  unsigned char* buffer;
  int width;
  int height;
  float bearing_x;
  float bearing_y;
};

using FTFacePtr = std::unique_ptr<FT_FaceRec_, FunctionWrapper<FT_Done_Face>>;
using GlyphPtr = std::unique_ptr<Glyph>;
class FreeTypeLib {
 public:
  FreeTypeLib();
  ~FreeTypeLib();

  FTFacePtr LoadFace(const string& font_name,
                     int pt_size,
                     int device_hdpi,
                     int device_vdpi);
  GlyphPtr Rasterize(FT_Face face, uint32_t glyph_index) const;

 private:
  FT_Library lib_;
  int force_ucs2_charmap(FT_Face face);
};
