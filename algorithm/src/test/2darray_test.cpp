#include "2darray.h"
#include "catch.hpp"


using namespace alg;
using namespace Catch::Matchers;

TEST_CASE("create a new array", "[create]") {
  Array2D<int> array(10, 10);
  REQUIRE(array.row() == 10);
  REQUIRE(array.col() == 10);
}

TEST_CASE("access element", "[access]") {
  Array2D<int> array(2, 3);
  for (int j = 0; j < 3; ++j) {
    array(0, j) = j;
  }

  SECTION("element at given row and column should correct"){
    REQUIRE(array(0,0) == 0);
    REQUIRE(array(0,1) == 1);
    REQUIRE(array(0,2) == 2);
  }

  SECTION("row at given index should correct"){
    auto row = array[0];
    REQUIRE(row[0] == 0);
    REQUIRE(row[1] == 1);
    REQUIRE(row[2] == 2);
  }
}
