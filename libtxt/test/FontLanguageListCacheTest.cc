//
// Created by rick on 2021/10/14.
//
#include <gtest/gtest.h>
#include "minikin/font_language_list_cache.h"
//#include "minikin/minikin_internal.h"

namespace minikin {
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