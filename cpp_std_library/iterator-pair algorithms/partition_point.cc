//
// Created by rick on 2022/1/17.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <array>


TEST(IteratorPairAlgorithems, PartitionPoint) {
  std::array<int, 9> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto is_even = [](int i) { return i % 2 == 0; };
  std::partition(v.begin(), v.end(), is_even);

  auto p = std::partition_point(v.begin(), v.end(), is_even);

  std::cout << "Before partition: ";
  std::copy(v.begin(), p, std::ostream_iterator<int>(std::cout, " "));
  std::cout << "\nAfter partition: ";
  std::copy(p, v.end(), std::ostream_iterator<int>(std::cout, " "));

  EXPECT_TRUE(!is_even(*p));

}

TEST(IteratorPairAlgorithems, PartitionCopy) {
  std::array<int, 11> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  auto is_even = [](int i) { return i % 2 == 0; };

  std::vector<int> evens{};
  std::vector<int> odds{};
  std::partition_copy(v.begin(),
                      v.end(),
                      std::back_inserter(evens),
                      std::back_inserter(odds),
                      is_even);

  EXPECT_THAT(events,testing::)
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();

}


