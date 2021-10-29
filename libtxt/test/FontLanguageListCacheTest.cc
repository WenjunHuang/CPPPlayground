//
// Created by rick on 2021/10/14.
//
#include <gtest/gtest.h>
#include <minikin/font_family.h>
#include <mutex>
#include "minikin/font_language_list_cache.h"
#include "minikin/minikin_internal.h"

namespace minikin {
TEST(FontLanguage, parse) {
  char en[] = "en";
  char cel[] = "cel";

  FontLanguage english(en, strlen(en));
  FontLanguage lang2(cel, strlen(cel));

  EXPECT_EQ((0x1F << 10) | ('e' - 'a') << 5 | ('n' - 'a'), english.language_);
  EXPECT_EQ(('c' - 'a') << 10 | ('e' - 'a') << 5 | ('l' - 'a'),
            lang2.language_);

  char zhHans[] = "zh-Hans";
  FontLanguage langRegion1(zhHans, strlen(zhHans));
  EXPECT_EQ(HB_TAG('H', 'a', 'n', 's'), langRegion1.script_);
  EXPECT_TRUE(langRegion1.sub_script_bits_ &
              (FontLanguage::kHanFlag | FontLanguage::kSimplifiedChineseFlag));

  char zhHansCN[] = "zh-Hans-CN";
  FontLanguage langStReg1(zhHansCN, strlen(zhHansCN));
  EXPECT_EQ((0x1F << 10) | ('C' - 'A') << 5 | ('N' - 'A'), langStReg1.region_);
}

TEST(FontLanguageListCacheTest, getId) {
  EXPECT_NE(0UL, FontStyle::registerLanguageList("en"));
  EXPECT_NE(0UL, FontStyle::registerLanguageList("jp"));
  EXPECT_NE(0UL, FontStyle::registerLanguageList("en,zh-Hans"));

  std::scoped_lock lock(gMinikinLock);
  EXPECT_EQ(0UL, FontLanguageListCache::getId(""));

  EXPECT_EQ(FontLanguageListCache::getId("en"),
            FontLanguageListCache::getId("en"));
  EXPECT_NE(FontLanguageListCache::getId("en"),
            FontLanguageListCache::getId("jp"));
}
TEST(FontLanguageListCacheTest, DefaultEmptyLanguageList) {
  //  std::lock_guard guard(gMinikinLock);
  auto inst = FontLanguageListCache::getInstance();
  EXPECT_FALSE(inst->language_lists_.empty());
  const auto& empty = inst->language_lists_[0];
  EXPECT_EQ(0, empty.size());

  const auto& first = inst->getById(0);
  EXPECT_EQ(0, first.size());

  const auto& f2 = inst->getId("");
  EXPECT_EQ(0, f2);

  // add first new language without script
  auto newId = inst->getId("zh-CN,zh-Hant-CN");
  EXPECT_EQ(1, newId);
  const auto& fc = inst->getById(newId);
  EXPECT_EQ(2, fc.size());
  EXPECT_STREQ("zh-Hans-CN", fc[0].getString().c_str());
  EXPECT_STREQ("zh-Hant-CN", fc[1].getString().c_str());
}

TEST(FontLanguageListCacheTest, SingleLanguage) {
  auto inst = FontLanguageListCache::getInstance();
}
}  // namespace minikin