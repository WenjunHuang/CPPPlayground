//
// Created by rick on 2021/10/11.
//

#pragma once
#include <skia/core/SkData.h>
#include "minikin/minikin_font.h"
namespace minikin {
class MinikinFontForTest : public MinikinFont {
 public:
  MinikinFontForTest(const std::string& font_path,
                     int index,
                     const std::vector<FontVariation>& variations);
  MinikinFontForTest(const std::string& font_path, int index)
      : MinikinFontForTest(font_path, index, {}) {}
  MinikinFontForTest(const std::string& font_path)
      : MinikinFontForTest(font_path, 0) {}

  MinikinFontForTest(MinikinFontForTest&&) = delete;
  MinikinFontForTest& operator=(MinikinFontForTest&&) = delete;
  hb_face_t* CreateHarfBuzzFace() const override;
  [[nodiscard]] float GetHorizontalAdvance(
      uint32_t glyph_id,
      const MinikinPaint& paint) const override;

  void GetBounds(MinikinRect* bounds,
                 uint32_t glyph_id,
                 const MinikinPaint& paint) const override;

  [[nodiscard]] const std::vector<minikin::FontVariation>& GetAxes()
      const override {
    return variations_;
  }

  [[nodiscard]] std::shared_ptr<MinikinFont> CreateFontWithVariation(
      const std::vector<FontVariation>& vector) const override;

  [[nodiscard]] const std::string& fontPath() const { return font_path_; }
  [[nodiscard]] const void* GetFontData() const { return font_data_->data(); }
  [[nodiscard]] size_t GetFontSize() const { return font_data_->size(); }
  [[nodiscard]] int GetFontIndex() const { return font_index_; }

 private:
  const std::string font_path_;
  const std::vector<FontVariation> variations_;
  const int font_index_;
  //  void* font_data_;
  sk_sp<SkData> font_data_;
  //  size_t font_size_;
};
}  // namespace minikin
