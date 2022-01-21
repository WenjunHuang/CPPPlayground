//
// Created by rick on 2021/10/27.
//
#include <gtest/gtest.h>
#include <minikin/font_language.h>
#include <minikin/font_language_list_cache.h>
#include <mutex>
#include "minikin/minikin_internal.h"
namespace minikin {

static FontLanguage createFontLanguage(const std::string& input) {
  std::scoped_lock lock(gMinikinLock);
  auto langId = FontLanguageListCache::getId(input);
  return FontLanguageListCache::getById(langId)[0];
}
TEST(FontLanguageTest, basicTests) {
  FontLanguage defaultLang;
  FontLanguage emptyLang("", 0);
  FontLanguage english = createFontLanguage("en");
  FontLanguage french = createFontLanguage("fr");
  FontLanguage und = createFontLanguage("und");
  FontLanguage undZsye = createFontLanguage("und-Zsye");

  EXPECT_EQ(english, english);
  EXPECT_EQ(french, french);

  EXPECT_TRUE(defaultLang != defaultLang);
  EXPECT_TRUE(emptyLang != emptyLang);
  EXPECT_TRUE(defaultLang != emptyLang);
  EXPECT_TRUE(defaultLang != und);
  EXPECT_TRUE(emptyLang != und);
  EXPECT_TRUE(english != defaultLang);
  EXPECT_TRUE(english != emptyLang);
  EXPECT_TRUE(english != french);
  EXPECT_TRUE(english != undZsye);
  EXPECT_TRUE(und != undZsye);
  EXPECT_TRUE(english != und);
}

TEST(FontLanguage, getString) {
  EXPECT_EQ("en-Latn-US", createFontLanguage("en").getString());
  EXPECT_EQ("en-Latn-US", createFontLanguage("en-Latn").getString());

  // Capitalized language code or lowercased script should be normalized.
  EXPECT_EQ("en-Latn-US", createFontLanguage("EN-LATN").getString());
  EXPECT_EQ("en-Latn-US", createFontLanguage("EN-latn").getString());
  EXPECT_EQ("en-Latn-US", createFontLanguage("en-latn").getString());

  // Invalid script should be kept.
  EXPECT_EQ("en-Xyzt-US", createFontLanguage("en-xyzt").getString());

  EXPECT_EQ("en-Latn-US", createFontLanguage("en-Latn-US").getString());
  EXPECT_EQ("ja-Jpan-JP", createFontLanguage("ja").getString());
  EXPECT_EQ("zh-Hant-TW", createFontLanguage("zh-TW").getString());
  EXPECT_EQ("zh-Hant-HK", createFontLanguage("zh-HK").getString());
  EXPECT_EQ("zh-Hant-MO", createFontLanguage("zh-MO").getString());
  EXPECT_EQ("zh-Hans-CN", createFontLanguage("zh").getString());
  EXPECT_EQ("zh-Hans-CN", createFontLanguage("zh-CN").getString());
  EXPECT_EQ("zh-Hans-SG", createFontLanguage("zh-SG").getString());
  //  EXPECT_EQ("und", createFontLanguage("und").getString());
  //  EXPECT_EQ("und", createFontLanguage("UND").getString());
  //  EXPECT_EQ("und", createFontLanguage("Und").getString());
  //  EXPECT_EQ("und-Zsye", createFontLanguage("und-Zsye").getString());
  //  EXPECT_EQ("und-Zsye", createFontLanguage("Und-ZSYE").getString());
  //  EXPECT_EQ("und-Zsye", createFontLanguage("Und-zsye").getString());

  EXPECT_EQ("de-Latn-DE", createFontLanguage("de-1901").getString());

  EXPECT_EQ("es-Latn-419", createFontLanguage("es-Latn-419").getString());

  // Emoji subtag is dropped from getString().
  EXPECT_EQ("es-Latn-419", createFontLanguage("es-419-u-em-emoji").getString());
  EXPECT_EQ("es-Latn-419",
            createFontLanguage("es-Latn-419-u-em-emoji").getString());
  // This is not a necessary desired behavior, just known behavior.
  EXPECT_EQ("en-Latn-US", createFontLanguage("und-Abcdefgh").getString());
}
TEST(FontLanguageTest, ScriptMatchTest) {
  const bool SUPPORTED = true;
  const bool NOT_SUPPORTED = false;

  struct TestCase {
    const std::string baseScript;
    const std::string requestedScript;
    bool isSupported;
  } testCases[] = {
      // Same scripts
      {"en-Latn", "Latn", SUPPORTED},
      {"ja-Jpan", "Jpan", SUPPORTED},
      {"ja-Hira", "Hira", SUPPORTED},
      {"ja-Kana", "Kana", SUPPORTED},
      {"ja-Hrkt", "Hrkt", SUPPORTED},
      {"zh-Hans", "Hans", SUPPORTED},
      {"zh-Hant", "Hant", SUPPORTED},
      {"zh-Hani", "Hani", SUPPORTED},
      {"ko-Kore", "Kore", SUPPORTED},
      {"ko-Hang", "Hang", SUPPORTED},
      {"zh-Hanb", "Hanb", SUPPORTED},

      // Japanese supports Hiragana, Katakanara, etc.
      {"ja-Jpan", "Hira", SUPPORTED},
      {"ja-Jpan", "Kana", SUPPORTED},
      {"ja-Jpan", "Hrkt", SUPPORTED},
      {"ja-Hrkt", "Hira", SUPPORTED},
      {"ja-Hrkt", "Kana", SUPPORTED},

      // Chinese supports Han.
      {"zh-Hans", "Hani", SUPPORTED},
      {"zh-Hant", "Hani", SUPPORTED},
      {"zh-Hanb", "Hani", SUPPORTED},

      // Hanb supports Bopomofo.
      {"zh-Hanb", "Bopo", SUPPORTED},

      // Korean supports Hangul.
      {"ko-Kore", "Hang", SUPPORTED},

      // Different scripts
      {"ja-Jpan", "Latn", NOT_SUPPORTED},
      {"en-Latn", "Jpan", NOT_SUPPORTED},
      {"ja-Jpan", "Hant", NOT_SUPPORTED},
      {"zh-Hant", "Jpan", NOT_SUPPORTED},
      {"ja-Jpan", "Hans", NOT_SUPPORTED},
      {"zh-Hans", "Jpan", NOT_SUPPORTED},
      {"ja-Jpan", "Kore", NOT_SUPPORTED},
      {"ko-Kore", "Jpan", NOT_SUPPORTED},
      {"zh-Hans", "Hant", NOT_SUPPORTED},
      {"zh-Hant", "Hans", NOT_SUPPORTED},
      {"zh-Hans", "Kore", NOT_SUPPORTED},
      {"ko-Kore", "Hans", NOT_SUPPORTED},
      {"zh-Hant", "Kore", NOT_SUPPORTED},
      {"ko-Kore", "Hant", NOT_SUPPORTED},

      // Hiragana doesn't support Japanese, etc.
      {"ja-Hira", "Jpan", NOT_SUPPORTED},
      {"ja-Kana", "Jpan", NOT_SUPPORTED},
      {"ja-Hrkt", "Jpan", NOT_SUPPORTED},
      {"ja-Hani", "Jpan", NOT_SUPPORTED},
      {"ja-Hira", "Hrkt", NOT_SUPPORTED},
      {"ja-Kana", "Hrkt", NOT_SUPPORTED},
      {"ja-Hani", "Hrkt", NOT_SUPPORTED},
      {"ja-Hani", "Hira", NOT_SUPPORTED},
      {"ja-Hani", "Kana", NOT_SUPPORTED},

      // Kanji doesn't support Chinese, etc.
      {"zh-Hani", "Hant", NOT_SUPPORTED},
      {"zh-Hani", "Hans", NOT_SUPPORTED},
      {"zh-Hani", "Hanb", NOT_SUPPORTED},

      // Hangul doesn't support Korean, etc.
      {"ko-Hang", "Kore", NOT_SUPPORTED},
      {"ko-Hani", "Kore", NOT_SUPPORTED},
      {"ko-Hani", "Hang", NOT_SUPPORTED},
      {"ko-Hang", "Hani", NOT_SUPPORTED},

      // Han with botomofo doesn't support simplified Chinese, etc.
      {"zh-Hanb", "Hant", NOT_SUPPORTED},
      {"zh-Hanb", "Hans", NOT_SUPPORTED},
      {"zh-Hanb", "Jpan", NOT_SUPPORTED},
      {"zh-Hanb", "Kore", NOT_SUPPORTED},
  };

  for (auto testCase : testCases) {
    hb_script_t script = hb_script_from_iso15924_tag(
        HB_TAG(testCase.requestedScript[0], testCase.requestedScript[1],
               testCase.requestedScript[2], testCase.requestedScript[3]));
    if (testCase.isSupported) {
      EXPECT_TRUE(
          createFontLanguage(testCase.baseScript).supportsHbScript(script))
          << testCase.baseScript << " should support "
          << testCase.requestedScript;
    } else {
      EXPECT_FALSE(
          createFontLanguage(testCase.baseScript).supportsHbScript(script))
          << testCase.baseScript << " shouldn't support "
          << testCase.requestedScript;
    }
  }
}
}  // namespace minikin