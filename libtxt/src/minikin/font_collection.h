//
// Created by rick on 2021/10/18.
//

#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <vector>
#include "font_family.h"

namespace minikin {
class FontCollection {
 private:
  explicit FontCollection();

 public:
  static std::shared_ptr<FontCollection> Create(
      const std::vector<std::shared_ptr<FontFamily>>& typefaces);

  // libtxt extension: an interface for looking up fallback fonts for characters
  // that do not match this collection's font families.
  class FallbackFontProvider {
   public:
    virtual ~FallbackFontProvider() = default;
    virtual const std::shared_ptr<FontFamily>& matchFallbackFont(
        uint32_t ch,
        std::string locale) = 0;
  };

  struct Run {
    FakedFont fakedFont;
    int start;
    int end;
  };

  void itemize(const uint16_t* string,
               size_t string_length,
               FontStyle style,
               std::vector<Run>* result) const;

  // Returns true if there is a glyph for the code point and variation selector
  // pair. Returns false if no fonts have a glyph for the code point and
  // variation selector pair, or invalid variation selector is passed.
  bool hasVariationSelector(uint32_t baseCodepoint,
                            uint32_t variationSelector) const;

  // Get base font with fakery information (fake bold could affect metrics)
  FakedFont baseFontFaked(FontStyle style);

  // Creates new FontCollection based on this collection while applying font
  // variations. Returns nullptr if none of variations apply to this collection.
  std::shared_ptr<FontCollection> createCollectionWithVariation(
      const std::vector<FontVariation>& variations);

  const std::unordered_set<AxisTag>& getSupportedTags() const {
    return supported_axes_;
  }

  uint32_t getId() const;

  void set_fallback_font_provider(std::unique_ptr<FallbackFontProvider> ffp){
    fallback_font_provider_ = std::move(ffp);
  }

 private:
  static constexpr int kLogCharsPerPage = 8;
  static constexpr int kPageMask = (1 << kLogCharsPerPage) - 1;

  // mFamilyVec holds the indices of the mFamilies and mRanges holds the range
  // of indices of mFamilyVec. The maximum number of pages is 0x10FF (U+10FFFF
  // >> 8). The maximum number of the fonts is 0xFF. Thus, technically the
  // maximum length of mFamilyVec is 0x10EE01 (0x10FF * 0xFF). However, in
  // practice, 16-bit integers are enough since most fonts supports only limited
  // range of code points.
  struct Range {
    uint16_t start;
    uint16_t end;
  };

  // Initialize the FontCollection.
  bool init(const std::vector<std::shared_ptr<FontFamily>>& typefaces);

  const std::shared_ptr<FontFamily>& getFamilyForChar(uint32_t ch,
                                                      uint32_t vs,
                                                      uint32_t langListId,
                                                      int variant) const;

  const std::shared_ptr<FontFamily>&
  findFallbackFont(uint32_t ch, uint32_t vs, uint32_t langListId) const;

  uint32_t calcFamilyScore(uint32_t ch,
                           uint32_t vs,
                           int variant,
                           uint32_t langListId,
                           const std::shared_ptr<FontFamily>& fontFamily) const;

  uint32_t calcCoverageScore(
      uint32_t ch,
      uint32_t vs,
      const std::shared_ptr<FontFamily>& fontFamily) const;

  static uint32_t calcLanguageMatchingScore(uint32_t userLangListId,
                                            const FontFamily& fontFamily);

  static uint32_t calcVariantMatchingScore(int variant,
                                           const FontFamily& fontFamily);

  // static for allocating unique id's
  static uint32_t next_id_;

  // unique id for this font collection (suitable for cache key)
  uint32_t id_;

  // Highest Utf-32 code point that can be mapped
  uint32_t max_char_;

  // This vector has pointers to the all font family instances in this
  // collection. This vector can't be empty.
  std::vector<std::shared_ptr<FontFamily>> families_;

  // Following two vectors are pre-calculated tables for resolving coverage
  // faster. For example, to iterate over all fonts which support Unicode code
  // point U+XXYYZZ, iterate font families index from
  // mFamilyVec[mRanges[0xXXYY].start] to mFamilyVec[mRange[0xXXYY].end] instead
  // of whole mFamilies. This vector contains indices into mFamilies. This
  // vector can't be empty.
  std::vector<Range> ranges_;
  std::vector<uint8_t> family_vec_;

  // This vector has pointers to the font family instances which have cmap 14
  // subtables.
  std::vector<std::shared_ptr<FontFamily>> vs_family_vec_;

  // Set of supported axes in this collection.
  std::unordered_set<AxisTag> supported_axes_;

  // libtxt extension: Fallback font provider.
  std::unique_ptr<FallbackFontProvider> fallback_font_provider_;

  // libtxt extension: Fallback fonts discovered after this font collection was
  // constructed.
  mutable std::map<std::string, std::vector<std::shared_ptr<FontFamily>>>
      cached_fallback_families_;
};

}  // namespace minikin