//
// Created by rick on 2021/9/22.
//

#pragma once
#include <harfbuzz/hb-ft.h>
#include <harfbuzz/hb.h>
#include "free_type_lib.h"
#include "glutils.h"

using namespace std;

struct HBText {
  std::string data;
  std::string language;
  hb_script_t script;
  hb_direction_t direction;
  const char* c_data() { return data.c_str(); }
};

namespace HBFeature {
const hb_tag_t kKernTag = HB_TAG('k', 'e', 'r', 'n');  // kerning operations
const hb_tag_t kLigaTag =
    HB_TAG('l', 'i', 'g', 'a');  // standard ligature substitution
const hb_tag_t kCligTag =
    HB_TAG('c', 'l', 'i', 'g');  // contextual ligature substitution

static hb_feature_t kLigatureOff = {kLigaTag, 0, 0,
                                    std::numeric_limits<unsigned int>::max()};
static hb_feature_t kLigatureOn = {kLigaTag, 1, 0,
                                   std::numeric_limits<unsigned int>::max()};
static hb_feature_t kKerningOff = {kKernTag, 0, 0,
                                   std::numeric_limits<unsigned int>::max()};
static hb_feature_t kKerningOn = {kKernTag, 1, 0,
                                  std::numeric_limits<unsigned int>::max()};
static hb_feature_t kCligOff = {kCligTag, 0, 0,
                                std::numeric_limits<unsigned int>::max()};
static hb_feature_t kCligOn = {kCligTag, 1, 0,
                               std::numeric_limits<unsigned int>::max()};
}  // namespace HBFeature

using HBFontPtr = std::unique_ptr<hb_font_t, FunctionWrapper<hb_font_destroy>>;
using HBBufferPtr =
    std::unique_ptr<hb_buffer_t, FunctionWrapper<hb_buffer_destroy>>;

class HBShaper {
 public:
  HBShaper(const string& font_file, std::unique_ptr<FreeTypeLib> lib)
      : lib_(std::move(lib)) {
    float size = 50;
    face_ = lib_->LoadFace(font_file, size * 64, 72, 72);
  }

  void Init() {
    font_ = HBFontPtr(hb_ft_font_create(face_.get(), nullptr));
    buffer_ = HBBufferPtr(hb_buffer_create());
    hb_buffer_allocation_successful(buffer_.get());
  }

  void AddFeature(hb_feature_t feature) { features_.push_back(feature); }

  std::vector<gl::Mesh> DrawText(HBText& text, float x, float y) {
    std::vector<gl::Mesh> meshes;

    hb_buffer_reset(buffer_.get());

    hb_buffer_set_direction(buffer_.get(), text.direction);
    hb_buffer_set_script(buffer_.get(), text.script);
    hb_buffer_set_language(
        buffer_.get(),
        hb_language_from_string(text.language.c_str(), text.language.length()));
    auto length = text.data.length();

    hb_buffer_add_utf8(buffer_.get(), text.c_data(), length, 0, length);

    // harfbuzz shaping
    hb_shape(font_.get(), buffer_.get(),
             features_.empty() ? nullptr : &features_[0], features_.size());

    unsigned int glyphCount;
    auto glyph_info = hb_buffer_get_glyph_infos(buffer_.get(), &glyphCount);
    auto glyph_pos = hb_buffer_get_glyph_positions(buffer_.get(), &glyphCount);

    for (int i = 0; i < glyphCount; i++) {
      auto glyph = lib_->Rasterize(face_.get(), glyph_info[i].codepoint);

      int twidth = pow(2, ceil(log(glyph->width) / log(2)));
      int theight = pow(2, ceil(log(glyph->height) / log(2)));

      auto tdata = std::make_unique<unsigned char[]>(twidth * theight);
      for (int iy = 0; iy < glyph->height; ++iy) {
        memcpy(tdata.get() + iy * twidth, glyph->buffer + iy * glyph->width,
               glyph->width);
      }

      // TODO: add debug log

      float s0 = 0.0;
      float t0 = 0.0;
      float s1 = (float)glyph->width / twidth;
      float t1 = (float)glyph->height / theight;
      float xa = (float)glyph_pos[i].x_advance / 64;
      float ya = (float)glyph_pos[i].y_advance / 64;
      float xo = (float)glyph_pos[i].x_offset / 64;
      float yo = (float)glyph_pos[i].y_offset / 64;
      float x0 = x + xo + glyph->bearing_x;
      float y0 = floor(y + yo + glyph->bearing_y);
      float x1 = x0 + glyph->width;
      float y1 = floor(y0 - glyph->height);

      auto vertices = std::make_unique<gl::Vertex[]>(4);
      vertices[0] = gl::Vertex(x0, y0, s0, t0);
      vertices[1] = gl::Vertex(x0, y1, s0, t1);
      vertices[2] = gl::Vertex(x1, y1, s1, t1);
      vertices[3] = gl::Vertex(x1, y0, s1, t0);

      auto indices = std::make_unique<unsigned short[]>(6);
      indices[0] = 0;
      indices[1] = 1;
      indices[2] = 2;
      indices[3] = 0;
      indices[4] = 2;
      indices[5] = 3;

      gl::GLTexturePtr texture_id(gl::CreateTexture(twidth, theight));
      gl::UploadTextureData(texture_id.get(), twidth, theight, tdata.get());

      meshes.push_back(gl::Mesh{
          std::move(indices),
          6,
          std::move(tdata),
          std::move(vertices),
          4,
          std::move(texture_id),
      });

      x += xa;
      y += ya;
    }
    return meshes;
  }

 private:
  std::unique_ptr<FreeTypeLib> lib_;
  FTFacePtr face_;
  HBBufferPtr buffer_;
  HBFontPtr font_;
  std::vector<hb_feature_t> features_;
};
