//
// Created by rick on 2021/10/23.
//
#include <fmt/printf.h>
#include <gtest/gtest.h>
#include <unicode/uchar.h>
#include <unicode/utypes.h>
int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(CodePointProperty, BidiControl) {
  EXPECT_TRUE(u_hasBinaryProperty(u'\u200E', UCHAR_BIDI_CONTROL)); // LRM
  EXPECT_FALSE(u_hasBinaryProperty(u'\u0061', UCHAR_BIDI_CONTROL));
}