//
// Created by rick on 2021/10/18.
//

#include "font_collection.h"
#include <mutex>
#include "log/log.h"
#include "minikin_internal.h"

namespace minikin {

// Special scores for the font fallback.
const uint32_t kUnsupportedFontScore = 0;
const uint32_t kFirstFontScore = UINT32_MAX;

bool FontCollection::init(
    const std::vector<std::shared_ptr<FontFamily>>& typefaces) {
  std::scoped_lock lock(gMinikinLock);
  id_ = next_id_++;
  std::vector<uint32_t> lastChar;
  auto nTypefaces = typefaces.size();

  const FontStyle defaultStyle;
  for (size_t i = 0; i < nTypefaces; i++) {
    const std::shared_ptr<FontFamily>& family = typefaces[i];
    if (family->getClosestMatch(defaultStyle).font == nullptr) {
      continue;
    }
    const SparseBitSet& coverage = family->getCoverage();
    families_.push_back(family);  // emplace_back would be better
    if (family->hasVSTable()) {
      vs_family_vec_.push_back(family);
    }
    max_char_ = std::max(max_char_, coverage.length());
    lastChar.push_back(coverage.nextSetBit(0));

    const std::unordered_set<AxisTag>& supportedAxes = family->supportedAxes();
    supported_axes_.insert(supportedAxes.begin(), supportedAxes.end());
  }
  nTypefaces = families_.size();
  if (nTypefaces == 0) {
    ALOGE("Font collection must have at least one valid typeface.");
    return false;
  }
  if (nTypefaces > 254) {
    ALOGE("Font collection may only have up to 254 font families.");
    return false;
  }
  size_t nPages = (max_char_ + kPageMask) >> kLogCharsPerPage;
  // TODO: Use variation selector map for mRanges construction.
  // A font can have a glyph for a base code point and variation selector pair
  // but no glyph for the base code point without variation selector. The family
  // won't be listed in the range in this case.
  for (size_t i = 0; i < nPages; i++) {
    Range dummy;
    ranges_.push_back(dummy);
    auto range = &ranges_.back();

    range->start = family_vec_.size();
    for (size_t j = 0; j < nTypefaces; j++) {
      if (lastChar[j] < (i + 1) << kLogCharsPerPage) {
        const std::shared_ptr<FontFamily>& family = families_[j];
        family_vec_.push_back(static_cast<uint8_t>(j));
        uint32_t nextChar =
            family->getCoverage().nextSetBit((i + 1) << kLogCharsPerPage);
        lastChar[j] = nextChar;
      }
    }
    range->end = family_vec_.size();
  }

  if (family_vec_.size() >= 0xFFFF) {
    ALOGE("Exceeded the maximum indexable cmap coverage.");
    return false;
  }
  return true;
}

// Implement heuristic for choosing best-match font. Here are the rules:
// 1. If first font in the collection has the character, it wins.
// 2. Calculate a score for the font family. See comments in calcFamilyScore for
// the detail.
// 3. Highest score wins, with ties resolved to the first font.
// This method never returns nullptr.
const std::shared_ptr<FontFamily>& FontCollection::getFamilyForChar(
    uint32_t ch,
    uint32_t vs,
    uint32_t langListId,
    int variant) const {
  if (ch >= max_char_) {
    // libtxt: check if the fallback font provider can match this character
    if (fallback_font_provider_) {
      const auto& fallback = findFallbackFont(ch, vs, langListId);
      if (fallback)
        return fallback;
    }
    return families_[0];
  }

  Range range = ranges_[ch >> kLogCharsPerPage];

  if (vs != 0) {
    range = {0, static_cast<uint16_t>(families_.size())};
  }

  int bestFamilyIndex = -1;
  uint32_t bestScore = kUnsupportedFontScore;
  for (size_t i = range.start; i < range.end; i++) {
//    const std::shared_ptr<FontFamily>& family =
//        vs == 0 ? families_[family_vec_[i]] : families_[i]
  }
}

}  // namespace minikin