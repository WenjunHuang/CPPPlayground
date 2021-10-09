//
// Created by rick on 2021/10/9.
//
#include <fmt/printf.h>
#include <gtest/gtest.h>
#include <unicode/unorm2.h>
#include <unicode/ustring.h>
#include <memory>
#include "util.h"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(Compose, ComposeTwoIntoOne) {
  UErrorCode status;
  auto normalizer = unorm2_getNFCInstance(&status);

  if (U_FAILURE(status)) {
    fmt::print("error {}", u_errorName(status));
  }

  UChar32 ret = unorm2_composePair(normalizer, U'\u006E', U'\u0307');
  EXPECT_EQ(ret, U'\U00001E45');
}

TEST(Compose, Decompose) {
  UErrorCode status;
  auto normalizer = unorm2_getNFCInstance(&status);

  if (U_FAILURE(status)) {
    fmt::print("error {}", u_errorName(status));
  }

  UChar decomposed[4];
  int len = unorm2_getRawDecomposition(normalizer, U'\U00001E45', decomposed, 4,
                                       &status);
  len = u_countChar32(decomposed, len);

  EXPECT_EQ(len, 2);

  len = 0;
  UChar32 code_point;
  U16_NEXT_UNSAFE(decomposed, len, code_point);
  EXPECT_EQ(code_point, U'\u006E');
  U16_NEXT_UNSAFE(decomposed, len, code_point);
  EXPECT_EQ(code_point, U'\u0307');
}