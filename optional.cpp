//
// Created by rick on 2017/7/24.
//
#include <iostream>
#include <ctime>
#include <cmath>
#include <boost/optional.hpp>
#include <gtest/gtest.h>

using boost::optional;
using namespace std;

optional<int> getEventRandomNumber() {
  int i = rand();
  return (i % 2 == 0) ? i : optional<int>{};
}

TEST(Optional, Demo1) {
  srand(static_cast<unsigned int>(time(0)));
  auto i = getEventRandomNumber();
  if (i)
    cout << sqrt(static_cast<float>(*i)) << endl;
}
