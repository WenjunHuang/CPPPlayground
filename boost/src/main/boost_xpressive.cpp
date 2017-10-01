#include <string>
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>
#include <gtest/gtest.h>

using namespace std;
using namespace boost;
using namespace boost::xpressive;

TEST(FactorialTest, Zero) {
  string s = "Boost Libraries";
  stringstream buffer;
  ostream_iterator<string> it{buffer};
  sregex expr = (+_w)[*boost::xpressive::ref(it) = _] >> _s >> +_w;

  EXPECT_TRUE(regex_match(s, expr));
  EXPECT_EQ("Boost1", buffer.str());
}


