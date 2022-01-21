//
// Created by rick on 2021/10/20.
//
#include <gtest/gtest.h>
#include <array>
#include "minikin/word_breaker.h"
#ifndef NELEM
#define NELEM(x) ((sizeof(x) / sizeof((x)[0])))
#endif

namespace minikin {
TEST(WordBreakerTest, basic) {
  uint16_t buf[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
  WordBreaker breaker;
  breaker.setLocale();
  breaker.setText(buf, sizeof(buf) / sizeof(buf[0]));
  EXPECT_EQ(0, breaker.current());
  EXPECT_EQ(6, breaker.next());       // after "hello, "
  EXPECT_EQ(0, breaker.wordStart());  // "hello"
  EXPECT_EQ(5, breaker.wordEnd());
  EXPECT_EQ(0, breaker.breakBadness());
  EXPECT_EQ(6, breaker.current());
  EXPECT_EQ(sizeof(buf) / sizeof(buf[0]), breaker.next());  // end
  EXPECT_EQ(6, breaker.wordStart());                        // "world"
  EXPECT_EQ(11, breaker.wordEnd());
  EXPECT_EQ(0, breaker.breakBadness());
  EXPECT_EQ(11, breaker.current());
}

TEST(WordBreakerTest, softHyphen) {
  char16_t buf[] = {'h', 'e', 'l', 0x00AD, 'l', 'o',
                    ' ', 'w', 'o', 'r',    'l', 'd'};
  WordBreaker breaker;
  breaker.setLocale();
  breaker.setText(reinterpret_cast<const uint16_t*>(buf), NELEM(buf));
  EXPECT_EQ(0, breaker.current());
  EXPECT_EQ(4, breaker.next());  // after "hel{SOFT HYPHEN}"
  EXPECT_EQ(0, breaker.wordStart());
}
}  // namespace minikin