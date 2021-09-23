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
  FreeTypeLib() {
    FT_Init_FreeType(&lib_);
  }
  ~FreeTypeLib(){
    FT_Done_FreeType(lib_);
  }

  FTFacePtr LoadFace(const string& font_name,
                     int pt_size,
                     int device_hdpi,
                     int device_vdpi){
    FT_Face f = nullptr;
    FT_Error status = FT_New_Face(lib_, font_name.c_str(), 0, &f);
    if (status != 0) {
      std::cerr << FT_Error_String(FT_Err_Unknown_File_Format) << std::endl;
      return nullptr;
    }
    FTFacePtr face(f);
    force_ucs2_charmap(face.get());
    FT_Set_Char_Size(face.get(), 0, pt_size, device_hdpi, device_vdpi);

    return face;
  }
  GlyphPtr Rasterize(FT_Face face, uint32_t glyph_index) const {
    FT_Int32 flags = FT_LOAD_DEFAULT;
    FT_Load_Glyph(face,
                  glyph_index,  // the glyph_index in the font file
                  flags);

    FT_GlyphSlot slot = face->glyph;
    auto result = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);

    FT_Bitmap ftBitmap = slot->bitmap;
    return std::make_unique<Glyph>(ftBitmap.buffer, ftBitmap.width, ftBitmap.rows,
                                   slot->bitmap_left, slot->bitmap_top);

  }

 private:
  FT_Library lib_;
  int force_ucs2_charmap(FT_Face face){
    for (int i = 0; i < face->num_charmaps; i++) {
      if (((face->charmaps[i]->platform_id == 0) &&
           (face->charmaps[i]->encoding_id == 3)) ||
          ((face->charmaps[i]->platform_id == 3) &&
           (face->charmaps[i]->encoding_id == 1))) {
        return FT_Set_Charmap(face, face->charmaps[i]);
      }
    }
    return -1;

  }
};
