//
// Created by rick on 2021/10/18.
//

#include "font_collection.h"
#include <unicode/uchar.h>
#include <mutex>
#include "emoji.h"
#include "log/log.h"
#include "minikin_internal.h"

namespace minikin {

// Special scores for the font fallback.
const uint32_t kUnsupportedFontScore = 0;
const uint32_t kFirstFontScore = UINT32_MAX;
const uint32_t NBSP = 0x00A0;
const uint32_t SOFT_HYPHEN = 0x00AD;
const uint32_t ZWJ = 0x200C;
const uint32_t ZWNJ = 0x200D;
const uint32_t HYPHEN = 0x2010;
const uint32_t NB_HYPHEN = 0x2011;
const uint32_t NNBSP = 0x202F;
const uint32_t FEMALE_SIGN = 0x2640;
const uint32_t MALE_SIGN = 0x2642;
const uint32_t STAFF_OF_AESCULAPIUS = 0x2695;

// Characters where we want to continue using existing font run instead of
// recomputing the best match in the fallback list.
static const uint32_t stickyAllowlist[] = {
    '!',   ',',         '-',       '.',
    ':',   ';',         '?',       NBSP,
    ZWJ,   ZWNJ,        HYPHEN,    NB_HYPHEN,
    NNBSP, FEMALE_SIGN, MALE_SIGN, STAFF_OF_AESCULAPIUS};

static bool isStickyAllowed(uint32_t c) {
  for (size_t i = 0; i < sizeof(stickyAllowlist) / sizeof(stickyAllowlist[0]);
       i++) {
    if (stickyAllowlist[i] == c)
      return true;
  }
  return false;
}
static bool isVariationSelector(uint32_t c) {
  return (0xFE00 <= c && c <= 0xFE0F) || (0xE0100 <= c && c <= 0xE01EF);
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
void FontCollection::itemize(const uint16_t* string,
                             size_t string_length,
                             FontStyle style,
                             std::vector<Run>* result) const {
  const uint32_t langListId = style.getLanguageListId();
  int variant = style.getVariant();
  const FontFamily* lastFamily = nullptr;
  Run* run = nullptr;

  if (string_length == 0) {
    return;
  }

  const uint32_t kEndOfString = 0xFFFFFFFF;

  uint32_t nextCh = 0;
  uint32_t prevCh = 0;
  size_t nextUtf16Pos = 0;
  size_t readLength = 0;
  U16_NEXT(string, readLength, string_length, nextCh);
  do {
    const uint32_t ch = nextCh;
    const size_t utf16Pos = nextUtf16Pos;
    nextUtf16Pos = readLength;
    if (readLength < string_length) {
      U16_NEXT(string, readLength, string_length, nextCh);
    } else {
      nextCh = kEndOfString;
    }

    bool shouldContinueRun = false;
    if (lastFamily != nullptr) {
      if (isStickyAllowed(ch)) {
        // Continue using existing font as long as it has coverage and is
        // allowed.
        shouldContinueRun = lastFamily->getCoverage().get(ch);
      } else if (ch == SOFT_HYPHEN || isVariationSelector(ch)) {
        // Always continue if the character is the soft hyphen or a variation
        // selector.
        shouldContinueRun = true;
      }
    }
    if (!shouldContinueRun) {
      const auto& family = getFamilyForChar(
          ch, isVariationSelector(nextCh) ? nextCh : 0, langListId, variant);
      if (utf16Pos == 0 || family.get() != lastFamily) {
        size_t start = utf16Pos;
        // Workaround for combining marks and emoji modifiers until we implement
        // per-cluster font selection: if a combining mark or an emoji modifier
        // is found in a different font that also supports the previous
        // character, attach previous character to the new run. U+20E3 COMBINING
        // ENCLOSING KEYCAP, used in emoji, is handled properly by this since
        // it's a combining mark too.
        if (utf16Pos != 0 &&
            ((U_GET_GC_MASK(ch) & U_GC_M_MASK) != 0 ||
             (isEmojiModifier(ch) && isEmojiBase(prevCh))) &&
            family != nullptr && family->getCoverage().get(prevCh)) {
          const size_t prevChLength = U16_LENGTH(prevCh);
          run->end -= prevChLength;
          if (run->start == run->end) {
            result->pop_back();
          }
          start -= prevChLength;
        }
        result->push_back(
            {family->getClosestMatch(style), static_cast<int>(start), 0});
        run = &result->back();
        lastFamily = family.get();
      }
    }
    prevCh = ch;
    run->end = nextUtf16Pos;  // exclusive
  } while (nextCh != kEndOfString);
}
uint32_t FontCollection::getId() const {
  return id_;
}

FontCollection::FontCollection() : max_char_(0) {}

std::shared_ptr<FontCollection> FontCollection::Create(
    const std::vector<std::shared_ptr<FontFamily>>& typefaces) {
  std::shared_ptr<FontCollection> font_collection(new FontCollection());
  if (!font_collection || !font_collection->init(typefaces))
    return nullptr;
  return font_collection;
}
bool FontCollection::init(
    const std::vector<std::shared_ptr<FontFamily>>& typefaces) {
  std::scoped_lock lock(gMinikinLock);
  id_ = next_id_++;
  std::vector<uint32_t> lastChar;
  size_t nTypefaces = typefaces.size();

  const FontStyle defaultStyle;
  for (size_t i = 0; i < nTypefaces; i++) {
    const auto& family = typefaces[i];
    if (family->getClosestMatch(defaultStyle).font == nullptr) {
      continue;
    }
    const auto& coverage = family->getCoverage();
    families_.push_back(family);
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
}

}  // namespace minikin