//
// Created by rick on 2022/1/10.
//
#include <vector>
#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>
#include <gmock/gmock.h>

class VectorTest : public ::testing::Test {
protected:
    void SetUp() override {
      vint_ = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      vstr_ = {"For", "each", "test", "defined", "with"};
    }

    std::vector<int> vint_;
    std::vector<std::string> vstr_;
};

TEST_F(VectorTest, IsIntVectorNotEmpty) {
  EXPECT_NE(vint_.size(), 0);
}

testing::AssertionResult IsEven(int n) {
  if ((n % 2) == 0)
    return testing::AssertionSuccess() << n << " is even";
  else
    return testing::AssertionFailure() << n << " is odd";
}

TEST(IsEven, IsEven) {
  EXPECT_FALSE(IsEven(5));
}

TEST(String, StringAssertions) {
  using testing::HasSubstr;
  using testing::MatchesRegex;
  std::string fooString{"This a needle on the ground"};
  ASSERT_THAT(fooString, HasSubstr("needle"));

  std::string barString{"slsfd91023"};
  EXPECT_THAT(barString, MatchesRegex(R"(\w*\d+)"));
}

TEST(Type, SameType) {
  testing::StaticAssertTypeEq<std::string, std::string>();
}

TEST(SkipTest, DoesSkip) {
  GTEST_SKIP() << "Skipping single test";
  EXPECT_EQ(0, 1);
}

class SkipFixture : public testing::Test {
protected:
    void SetUp() override {
      GTEST_SKIP() << "Skipping all tests for this fixture";
    }
};

TEST_F(SkipFixture, SkipsOneTest) {
  EXPECT_EQ(5, 7);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

