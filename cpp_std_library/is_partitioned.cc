//
// Created by rick on 2022/1/11.
//
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

TEST(is_partitioned, ReturnTrueWhenRangeIsWellPartitioned) {
  std::vector<int> vec{2, 4, 6, 8, 10, 1, 3, 5, 6, 9};
  EXPECT_TRUE(
    std::is_partitioned(std::begin(vec),
                        std::end(vec),
                        [](auto &v) {
                            return v % 2 == 0;
                        }));
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
}

