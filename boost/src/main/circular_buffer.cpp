//
// Created by xxzyjy on 27/07/2017.
//

#include <boost/circular_buffer.hpp>
#include <iostream>
#include <gtest/gtest.h>

using namespace std;
using namespace boost;

TEST(CircularBuffer, Simple) {
  using circular_buffer = circular_buffer<int>;

  circular_buffer cb{3};

  EXPECT_EQ(3, cb.capacity());
  EXPECT_EQ(0, cb.size());

  cb.push_back(0);
  cb.push_back(1);
  cb.push_back(2);
  cb.push_back(3);

  EXPECT_EQ(3, cb.capacity());
  EXPECT_EQ(3, cb.size());

  circular_buffer::array_range r1,r2;
  r1 = cb.array_one();
  r2 = cb.array_two();
}

CircularBuffer_Simple_Test::CircularBuffer_Simple_Test(const CircularBuffer_Simple_Test &) {

}
