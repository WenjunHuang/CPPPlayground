//
// Created by rick on 2021/10/20.
//
#include <gtest/gtest.h>
#include "minikin/word_breaker.h"
namespace minikin {
TEST(WordBreakerTest, basic) {
  uint16_t buf[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
  WordBreaker breaker;
  breaker.setLocale();
  breaker.setText(buf, sizeof(buf) / sizeof(buf[0]));
  EXPECT_EQ(0, breaker.current());
  EXPECT_EQ(6, breaker.next());       // after "hello "
  EXPECT_EQ(0, breaker.wordStart());  // "hello"
  //  EXPECT_EQ(5,breaker.wordEnd());
}
}  // namespace minikin