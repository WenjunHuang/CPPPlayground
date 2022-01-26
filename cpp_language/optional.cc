//
// Created by rick on 2021/12/10.
//
#include <gtest/gtest.h>
#include <optional>

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(Option, OptionWithRef) {
  using std::optional;
  optional<int> i_1;
  ASSERT_FALSE(i_1);

  optional<int&> ref_1 = i_1;
}