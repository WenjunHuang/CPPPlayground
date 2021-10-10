//
// Created by HUANG WEN JUN on 2021/9/20.
//
#include <gtest/gtest.h>
#include "bidi_iterator_icu.h"
#include "break_iterator_icu.h"

TEST(Bidi, CreateBidiIteratorICUWithUtf16) {
  char16_t utf16[] =
      u"he said \"\u0671\u0679!\u200f\" to her\nthis is the second paragraph";
  auto result = BidiIterator_icu::makeBidiIterator(
      utf16, sizeof(utf16) / sizeof(utf16[0]), BidiIterator::Direction::kLTR);
  ASSERT_TRUE(result);

  EXPECT_EQ(result->getLevelAt(0), 0);
  EXPECT_EQ(result->getLevelAt(10), 1);
  EXPECT_EQ(result->getLevelAt(10), 1);
  EXPECT_EQ(result->getLevelAt(16), 0);
}
TEST(Bidi, CreateBidiIteratorICUWithUtf8) {
  char utf8[] =
      u8"he said \"\u0671\u0679!\u200f\" to her\nthis is the second paragraph";
  auto result = BidiIterator_icu::makeBidiIterator(
      utf8, sizeof(utf8) / sizeof(utf8[0]), BidiIterator::Direction::kLTR);
  ASSERT_TRUE(result);
  EXPECT_EQ(result->getLevelAt(0), 0);
  EXPECT_EQ(result->getLevelAt(10), 1);
  EXPECT_EQ(result->getLevelAt(10), 1);
  EXPECT_EQ(result->getLevelAt(16), 0);
}

TEST(BreakIterator,CreateABreakIterator){
  auto& cache = IcuBreakIteratorCache::get();
  auto iterator = cache.makeBreakIterator(Unicode::BreakType::kGraphemes);
  ASSERT_TRUE(iterator);
}
