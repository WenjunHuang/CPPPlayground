//
// Created by rick on 2021/9/2.
//
#include <gtest/gtest.h>
#include <queue>
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

class QueueTest : public ::testing::Test {
 protected:
  void SetUp() override {
    _q1.push(1);
    _q2.push(2);
    _q2.push(3);
  }

  std::queue<int> _q0;
  std::queue<int> _q1;
  std::queue<int> _q2;
};

TEST_F(QueueTest, IsEmptyInitialy) {
  EXPECT_EQ(_q0.size(), 0);
}

TEST_F(QueueTest, DequeueWorks) {
  int n = _q1.front();
  _q1.pop();
  EXPECT_EQ(n, 1);
}