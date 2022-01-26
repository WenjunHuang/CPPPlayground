//
// Created by rick on 2022/1/10.
//

#include <gtest/gtest.h>
#include <vector>

TEST(BidiIterator, FirstMinusOneEqualsCauseDeath) {
  std::vector<int> vec{1, 2, 3, 4};
  EXPECT_DEATH({
                 vec.begin() - 1;
               }, "Assert");
}

TEST(BidiIteraotr, EndMinus1IsTheLastElement) {
  std::vector<int> vec{1, 2, 3, 4};
  EXPECT_EQ(*(std::end(vec) - 1), 4);
}

TEST(ReverseIterator, RBeginEqualsToEnd) {
  std::vector<int> vec{1, 2, 3, 4};
  auto rbegin = std::reverse_iterator(std::end(vec));
  auto rend = std::reverse_iterator(std::begin(vec));

  EXPECT_EQ(rbegin.base(), std::end(vec));

//  EXPECT_EQ(rend.base(), std::end(vec));
}

void NormalExit() {
  exit(1);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
}