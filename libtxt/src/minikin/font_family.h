//
// Created by rick on 2021/10/11.
//

#pragma once

#include <cstdint>
namespace minikin {
class MinikinFont;

// FontStyle represents all style information needed to select an actual font
// from a collection. The implementation is packed into two 32-bit words
// so it can be efficiently copied, embedded in other objects, etc.
class FontStyle {
 public:
  FontStyle()
      : FontStyle(0 /* variant */, 4 /* weight */, false /* italic */) {}
  FontStyle(int weight, bool italic) : FontStyle(0, weight, italic) {}
  FontStyle(uint32_t langListId) : FontStyle(langListId, 0, 4, false) {}
  FontStyle(int variant, int weight, bool italic);
  FontStyle(uint32_t langListId, int variant, int weight, bool italic);

  int getWeight() const { return bits_ & kWeightMask; }
  bool getItalic() const { return (bits_ & kItalicMask) != 0; }
  int getVariant() const { return (bits_ >> kVariantShift) & kVariantMask; }
  uint32_t getLanguageListId() const { return language_list_id_; }

  bool operator==(const FontStyle& other) const {
    return bits_ == other.bits_ && language_list_id_ == language_list_id_;
  }

 private:
  static constexpr uint32_t kWeightMask = (1 << 4) - 1;
  static constexpr uint32_t kItalicMask = 1 << 4;
  static constexpr int kVariantShift = 5;
  static constexpr uint32_t kVariantMask = (1 << 2) - 1;
  uint32_t bits_;
  uint32_t language_list_id_;
};

using AxisTag = uint32_t;
// attributes representing transforms (fake bold, fake italic) to match styles
class FontFakery {
 public:
  FontFakery() : fake_italic_(false), fake_bold_(false) {}
  FontFakery(bool fakeBold, bool fakeItalic)
      : fake_italic_(fakeItalic), fake_bold_(fakeBold) {}

  bool isFakeBold() { return fake_bold_; }
  bool isFakeItalic() { return fake_italic_; }

 private:
  bool fake_bold_;
  bool fake_italic_;
};

struct FontVariation {
  FontVariation(AxisTag axisTag, float value)
      : axisTag(axisTag), value(value) {}
  AxisTag axisTag;
  float value;
};

class FontFamily {};
}  // namespace minikin