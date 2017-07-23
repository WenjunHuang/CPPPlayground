#include <iostream>
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>
#include "gtest/gtest.h"


int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
}