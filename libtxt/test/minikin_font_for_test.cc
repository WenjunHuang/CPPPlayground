//
// Created by rick on 2021/10/11.
//

#include "minikin_font_for_test.h"
#include <skia/core/SkData.h>

namespace minikin {
static int uniqueId = 0;

MinikinFontForTest::MinikinFontForTest(
    const std::string& font_path,
    int index,
    const std::vector<FontVariation>& variations)
    : MinikinFont(uniqueId++),
      font_path_(font_path),
      variations_(variations),
      font_index_(index) {
  font_data_ = SkData::MakeFromFileName(font_path_.c_str());
  if (!font_data_)
    throw std::exception("can not open font file");
}

float MinikinFontForTest::GetHorizontalAdvance(
    uint32_t glyph_id,
    const MinikinPaint& paint) const {
  return 10.0f;
}

void MinikinFontForTest::GetBounds(MinikinRect* bounds,
                                   uint32_t glyph_id,
                                   const MinikinPaint& paint) const {
  bounds->mLeft = 0.0f;
  bounds->mRight = 0.0f;
  bounds->mTop = 0.0f;
  bounds->mBottom = 0.0f;
}

std::shared_ptr<MinikinFont> MinikinFontForTest::CreateFontWithVariation(
    const std::vector<FontVariation>& vector) const {
  return std::shared_ptr<MinikinFont>(
      new MinikinFontForTest(font_path_, font_index_, variations_));
}

hb_face_t* MinikinFontForTest::CreateHarfBuzzFace() const {
  auto blob =
      hb_blob_create((const char*)font_data_->data(), font_data_->size(),
                     HB_MEMORY_MODE_READONLY, nullptr, nullptr);
  return hb_face_create(blob, font_index_);
}

}  // namespace minikin