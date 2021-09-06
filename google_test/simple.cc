//
// Created by rick on 2021/9/2.
//
#include <gtest/gtest.h>
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

int Factorial(int n) {
  assert(n >= 0);
  if (n == 0)
    return 1;
  else
    return n * Factorial(n - 1);
}

TEST(FactorialTest, HandlesZeroInput) {
  EXPECT_EQ(Factorial(0), 1);
}

TEST(FactorialTest, HandlesPositiveInput) {
  EXPECT_EQ(Factorial(1), 1);
  EXPECT_EQ(Factorial(2), 2);
  EXPECT_EQ(Factorial(3), 6);
}

void deathFunction() {
  std::cerr << "death" << std::endl;
  exit(1);
}
TEST(EXPECT, EXPECT_DEATH_IF_SUPPORTED) {
  EXPECT_DEATH_IF_SUPPORTED(deathFunction(), "death");
}