//
// Created by rick on 2022/1/10.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>

QString fizzBuzz(int number) {
  QString result{};

  if (number % 3 == 0) {
    result.append("Fizz");
  }
  if (number % 5 == 0)
    result.append("Buzz");

  if (!result.isEmpty())
    return result;
  else
    return QString::number(number);
}

TEST(Kata, FizzBuzz) {
  EXPECT_EQ(fizzBuzz(1), QString::number(1));
  EXPECT_EQ(fizzBuzz(2), QString::number(2));
  EXPECT_EQ(fizzBuzz(3), QString{"Fizz"});
  EXPECT_EQ(fizzBuzz(5), QString{"Buzz"});
  EXPECT_EQ(fizzBuzz(15), QString{"Fizz Buzz"});
}

int main(int argc, char *argv[]) {
  testing::InitGoogleMock(&argc, argv);
  RUN_ALL_TESTS();
}
