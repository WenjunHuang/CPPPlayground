//
// Created by rick on 2022/1/10.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QVector>
#include <vector>

template<typename T> using Container = std::vector<T>;

Container<int> primeFactors(int value) {
  Container<int> result{};

  while (value > 1) {
    auto divisor = 2;
    while (divisor <= value) {
      while (0 == value % divisor) {
        result.push_back(divisor);
        value /=divisor;
      }
      ++divisor;
    }
  }

  return result;
}

TEST(PrimeFactors, CalculatesThePrimeFactorsOfAnInteger) {
  using testing::IsEmpty;
  using testing::ContainerEq;

  EXPECT_THAT(primeFactors(1), IsEmpty());
  EXPECT_THAT(primeFactors(2), ContainerEq(Container<int>{2}));
  EXPECT_THAT(primeFactors(3), ContainerEq(Container<int>{3}));
  EXPECT_THAT(primeFactors(4), ContainerEq(Container<int>{2, 2}));
  EXPECT_THAT(primeFactors(5), ContainerEq(Container<int>{5}));
  EXPECT_THAT(primeFactors(6), ContainerEq(Container<int>{2, 3}));
  EXPECT_THAT(primeFactors(9), ContainerEq(Container<int>{3, 3}));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
