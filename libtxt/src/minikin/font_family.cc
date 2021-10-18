//
// Created by rick on 2021/10/11.
//

#include "font_family.h"
#include "cmap_coverage.h"
#include "font_language_list_cache.h"
#include "font_utils.h"
#include "minikin_font.h"
#include "minikin_internal.h"
#include "utils/JenkinsHash.h"

namespace minikin {
FontStyle::FontStyle(int variant, int weight, bool italic)
    : FontStyle(FontLanguageListCache::kEmptyListId, variant, weight, italic) {}

FontStyle::FontStyle(uint32_t langListId, int variant, int weight, bool italic)
    : bits_(pack(variant, weight, italic)), language_list_id_(langListId) {}

uint32_t FontStyle::pack(int variant, int weight, bool italic) {
  return (weight & kWeightMask) | (italic ? kItalicMask : 0) |
         (variant << kVariantShift);
}
android::hash_t FontStyle::hash() const {
  uint32_t hash = android::JenkinsHashMix(0, bits_);
  hash = android::JenkinsHashMix(hash, language_list_id_);
  return android::JenkinsHashWhiten(hash);
}

uint32_t FontStyle::registerLanguageList(const std::string& languages) {
  std::scoped_lock lock(gMinikinLock);
  return FontLanguageListCache::getId(languages);
}

Font::Font(std::shared_ptr<MinikinFont> typeface, FontStyle style)
    : typeface(std::move(typeface)), style(style) {}

std::unordered_set<AxisTag> Font::getSupportedAxesLocked() const {
  auto fvarTag = MinikinFont::MakeTag('f', 'v', 'a', 'r');
  HbBlob fvarTable(getFontTable(typeface.get(), fvarTag));
  if (fvarTable.size() == 0) {
    return {};
  }

  std::unordered_set<AxisTag> supportedAxes;
  analyzeAxes(fvarTable.get(), fvarTable.size(), &supportedAxes);
  return supportedAxes;
}

FontFamily::FontFamily(std::vector<Font>&& fonts)
    : FontFamily(0, std::move(fonts)) {}

FontFamily::FontFamily(int variant, std::vector<Font>&& fonts)
    : FontFamily(FontLanguageListCache::kEmptyListId,
                 variant,
                 std::move(fonts)) {}

FontFamily::FontFamily(uint32_t langId, int variant, std::vector<Font>&& fonts)
    : lang_id_(langId),
      variant_(variant),
      fonts_(std::move(fonts)),
      has_vs_table_(false) {
  computeCoverage();
}
bool FontFamily::analyzeStyle(const std::shared_ptr<MinikinFont>& typeface,
                              int* weight,
                              bool* italic) {
  std::scoped_lock lock(gMinikinLock);
  auto os2Tag = MinikinFont::MakeTag('O', 'S', '/', '2');
  HbBlob os2Table(getFontTable(typeface.get(), os2Tag));
  if (os2Table.get() == nullptr)
    return false;
  return ::minikin::analyzeStyle(os2Table.get(), os2Table.size(), weight,
                                 italic);
}

static FontFakery computeFakery(FontStyle wanted, FontStyle actual) {
  // If desired weight is semibold or darker, and 2 or more grades
  // higher than actual (for example, medium 500 -> bold 700), then
  // select fake bold.
  int wantedWeight = wanted.getWeight();
  bool isFakeBold =
      wantedWeight >= 6 && (wantedWeight - actual.getWeight()) >= 2;
  bool isFakeItalic = wanted.getItalic() && !actual.getItalic();
  return FontFakery(isFakeBold, isFakeItalic);
}

// Compute a matching metric between two styles - 0 is an exact match
static int computeMatch(FontStyle style1, FontStyle style2) {
  if (style1 == style2)
    return 0;
  int score = abs(style1.getWeight() - style2.getWeight());
  if (style1.getItalic() != style2.getItalic())
    score += 2;
  return score;
}

FakedFont FontFamily::getClosestMatch(FontStyle style) const {
  const Font* bestFont = nullptr;
  int bestMatch = 0;
  for (size_t i = 0; i < fonts_.size(); i++) {
    const auto& font = fonts_[i];
    int match = computeMatch(font.style, style);
    if (i == 0 || match < bestMatch) {
      bestFont = &font;
      bestMatch = match;
    }
  }

  if (bestFont != nullptr) {
    return FakedFont{bestFont->typeface.get(),
                     computeFakery(style, bestFont->style)};
  }
  return FakedFont{nullptr, FontFakery()};
}
bool FontFamily::isColorEmojiFamily() const {
  const auto& languageList = FontLanguageListCache::getById(lang_id_);
  for (size_t i = 0; i < languageList.size(); ++i) {
    if (languageList[i].getEmojiStyle() == FontLanguage::EMSTYLE_EMOJI) {
      return true;
    }
  }
  return false;
}
bool FontFamily::hasGlyph(uint32_t codepoint, uint32_t variantSelector) const {
  assertMinikinLocked();
  if (variantSelector != 0 && !has_vs_table_) {
    // Early exit if the variation selector is specified but the font doesn't
    // have a cmap format 14 subtable.
    return false;
  }

  const FontStyle defaultStyle;
  auto font = getHbFontLocked(getClosestMatch(defaultStyle).font);
  uint32_t unusedGlyph;
  bool result =
      hb_font_get_glyph(font, codepoint, variantSelector, &unusedGlyph);
  hb_font_destroy(font);
  return result;
}
std::shared_ptr<FontFamily> FontFamily::createFamilyWithVariation(
    const std::vector<FontVariation>& variations) const {
  if (variations.empty() || supported_axes_.empty()) {
    return nullptr;
  }

  bool hasSupportedAxis = false;
  for (const auto& variation : variations) {
    if (supported_axes_.find(variation.axisTag) != supported_axes_.end()) {
      hasSupportedAxis = true;
      break;
    }
  }
  if (!hasSupportedAxis) {
    // None of variation axes are supported by this family.
    return nullptr;
  }

  std::vector<Font> fonts;
  for (const auto& font : fonts_) {
    bool supportedVariations = false;
    std::scoped_lock lock(gMinikinLock);
    std::unordered_set<AxisTag> supportedAxes = font.getSupportedAxesLocked();
    if (!supportedAxes.empty()) {
      for (const auto& variation : variations) {
        if (supportedAxes.find(variation.axisTag) != supportedAxes.end()) {
          supportedVariations = true;
          break;
        }
      }
    }
    std::shared_ptr<MinikinFont> minikinFont;
    if (supportedVariations) {
      minikinFont = font.typeface->CreateFontWithVariation(variations);
    }
    if (minikinFont == nullptr) {
      minikinFont = font.typeface;
    }
    fonts.emplace_back(std::move(minikinFont), font.style);
  }

  return std::make_shared<FontFamily>(lang_id_, variant_, std::move(fonts));
}
void FontFamily::computeCoverage() {
  std::scoped_lock lock(gMinikinLock);
  const FontStyle defaultStyle;
  const MinikinFont* typeface = getClosestMatch(defaultStyle).font;
  const uint32_t cmapTag = MinikinFont::MakeTag('c', 'm', 'a', 'p');
  HbBlob cmapTable(getFontTable(typeface, cmapTag));
  if (cmapTable.get() == nullptr) {
    // Missing or corrupt font cmap table; bail out.
    // The cmap table maps charcodes to glyph indices in a font.
    return;
  }
  coverage_ = CmapCoverage::getCoverage(cmapTable.get(), cmapTable.size(),
                                        &has_vs_table_);
  for (size_t i = 0; i < fonts_.size(); ++i) {
    std::unordered_set<AxisTag> supportedAxes =
        fonts_[i].getSupportedAxesLocked();
    supported_axes_.insert(supportedAxes.begin(), supportedAxes.end());
  }
}

}  // namespace minikin
