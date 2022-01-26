//
// Created by rick on 2021/10/18.
//

#include <gtest/gtest.h>
#include <minikin/font_collection.h>

#include "minikin/minikin_internal.h"
#include "minikin_font_for_test.h"

namespace minikin {
// The test font has following glyphs.
// U+82A6
// U+82A6 U+FE00 (VS1)
// U+82A6 U+E0100 (VS17)
// U+82A6 U+E0101 (VS18)
// U+82A6 U+E0102 (VS19)
// U+845B
// U+845B U+FE01 (VS2)
// U+845B U+E0101 (VS18)
// U+845B U+E0102 (VS19)
// U+845B U+E0103 (VS20)
// U+537F
// U+717D U+FE02 (VS3)
// U+717D U+E0102 (VS19)
// U+717D U+E0103 (VS20)
const char kVsTestFont[] = "fonts/VariationSelectorTest-Regular.ttf";

void expectVSGlyphs(const FontCollection* fc,
                    uint32_t codepoint,
                    const std::set<uint32_t>& vsSet) {
  for (uint32_t vs = 0xFE00; vs <= 0xE01EF; ++vs) {
    // Move to variation selectors supplements after variation selectors.
    if (vs == 0xFF00) {
      vs = 0xE0100;
    }
    if (vsSet.find(vs) == vsSet.end()) {
      EXPECT_FALSE(fc->hasVariationSelector(codepoint, vs))
          << "Glyph for U+" << std::hex << codepoint << " U+" << vs;
    } else {
      EXPECT_TRUE(fc->hasVariationSelector(codepoint, vs))
          << "Glyph for U+" << std::hex << codepoint << " U+" << vs;
    }
  }
}

TEST(FontCollectionTest, hasVariationSelectorTest) {
  std::shared_ptr<MinikinFont> font(new MinikinFontForTest(kVsTestFont));
  std::shared_ptr<FontFamily> family(
      new FontFamily(std::vector<Font>({Font(font, FontStyle())})));
  std::vector<std::shared_ptr<FontFamily>> families({family});
  std::shared_ptr<FontCollection> fc = FontCollection::Create(families);

  EXPECT_FALSE(fc->hasVariationSelector(0x82A6, 0));
  expectVSGlyphs(
      fc.get(), 0x82A6,
      std::set<uint32_t>({0xFE00, 0xFE0E, 0xE0100, 0xE0101, 0xE0102}));
}
}  // namespace minikin