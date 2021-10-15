//
// Created by rick on 2021/10/11.
//

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_set>
#include "SparseBitSet.h"
#include "utils/TypeHelpers.h"

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

  [[nodiscard]] int getWeight() const { return bits_ & kWeightMask; }
  [[nodiscard]] bool getItalic() const { return (bits_ & kItalicMask) != 0; }
  [[nodiscard]] int getVariant() const {
    return (bits_ >> kVariantShift) & kVariantMask;
  }
  [[nodiscard]] uint32_t getLanguageListId() const { return language_list_id_; }

  bool operator==(const FontStyle& other) const {
    return bits_ == other.bits_ && language_list_id_ == other.language_list_id_;
  }

  android::hash_t hash() const;

  // Looks up a language list from an internal cache and returns its ID.
  // If the passed language list is not in the cache, registers it and returns
  // newly assigned ID.
  [[maybe_unused]] static uint32_t registerLanguageList(
      const std::string& languages);

 private:
  static constexpr uint32_t kWeightMask = (1 << 4) - 1;
  static constexpr uint32_t kItalicMask = 1 << 4;
  static constexpr int kVariantShift = 5;
  static constexpr uint32_t kVariantMask = (1 << 2) - 1;
  uint32_t bits_;
  uint32_t language_list_id_;

  static uint32_t pack(int variant, int weight, bool italic);
};

enum FontVariant {
  VARIANT_DEFAULT = 0,
  VARIANT_COMPACT = 1,
  VARIANT_ELEGANT = 2,
};

inline android::hash_t hash_type(const FontStyle& style) {
  return style.hash();
}

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

struct FakedFont {
  // ownership is the enclosing FontCollection
  MinikinFont* font;
  FontFakery fakery;
};

using AxisTag = uint32_t;
struct Font {
  Font(std::shared_ptr<MinikinFont> typeface, FontStyle style);

  std::shared_ptr<MinikinFont> typeface;
  FontStyle style;

  [[nodiscard]] std::unordered_set<AxisTag> getSupportedAxesLocked() const;
};

struct FontVariation {
  FontVariation(AxisTag axisTag, float value)
      : axisTag(axisTag), value(value) {}
  AxisTag axisTag;
  float value;
};

class FontFamily {
 public:
  explicit FontFamily(std::vector<Font>&& fonts);
  FontFamily(int variant, std::vector<Font>&& fonts);
  FontFamily(uint32_t langId, int variant, std::vector<Font>&& fonts);

  static bool analyzeStyle(const std::shared_ptr<MinikinFont>& typeface,
                           int* weight,
                           bool* italic);

  [[nodiscard]] FakedFont getClosestMatch(FontStyle style) const;
  [[nodiscard]] uint32_t langId() const { return lang_id_; }
  [[nodiscard]] int variant() const { return variant_; }

  // API's for enumerating the fonts in a family. These don't guarantee any
  // particular order
  [[nodiscard]] size_t getNumFonts() const { return fonts_.size(); }
  [[nodiscard]] const std::shared_ptr<MinikinFont>& getFont(size_t index) const {
    return fonts_[index].typeface;
  }

  [[nodiscard]] FontStyle getStyle(size_t index) const { return fonts_[index].style; }
  [[nodiscard]] bool isColorEmojiFamily() const;
  [[nodiscard]] const std::unordered_set<AxisTag>& supportedAxes() const {
    return supported_axes_;
  }

  // Get Unicode coverage.
  [[nodiscard]] const SparseBitSet& getCoverage() const { return coverage_; }

  // Returns true if the font has a glyph for the code point and variation
  // selector pair. Caller should acquire a lock before calling the method.
  [[nodiscard]] bool hasGlyph(uint32_t codepoint, uint32_t variantSelector) const;

  // Returns true if this font family has a variation sequence table (cmap
  // format14 subtable).
  [[nodiscard]] bool hasVSTable() const { return has_vs_table_; }

  // Creates new FontFamily based on this family while applying font variations.
  // Returns nullptr if none of variations apply to this family.
  std::shared_ptr<FontFamily> createFamilyWithVariation(
      const std::vector<FontVariation>& variations) const;

 private:
  void computeCoverage();

  uint32_t lang_id_;
  int variant_;
  std::vector<Font> fonts_;
  std::unordered_set<AxisTag> supported_axes_;
  SparseBitSet coverage_;
  bool has_vs_table_;
};
}  // namespace minikin
