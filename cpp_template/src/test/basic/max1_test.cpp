#include "basics/max1.h"
#include "catch.hpp"
#include <string>
#include <complex>

using namespace Catch::Matchers;
TEST_CASE("template method", "[template]") {
  auto m = ::max(10, 20);
  REQUIRE(m == 20);

  auto f = ::max(10.0,20.0);
  REQUIRE(f == 20.0);

  auto str = ::max(std::string("mathematics"),std::string("math"));
  REQUIRE(str.compare(std::string("mathematics")) ==0 );
}

TEST_CASE("void as template argument","[template]"){
  void *vp = nullptr;
  ::foo(vp);
}

TEST_CASE("faile to instantiate a template","[template]"){
//  std::complex<float> c1,c2;
//  ::max(c1, c2);
}

