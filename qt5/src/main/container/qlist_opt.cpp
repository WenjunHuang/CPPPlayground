//
// Created by xxzyjy on 26/10/2017.
//
#include "catch.hpp"
#include <QList>
#include <algorithm>

using namespace Catch::Matchers;
TEST_CASE("qlist_opt","[QList]") {
  {
    QList<QString> list;
    list << QString("This is a test string");
    REQUIRE(list.size() == 1);
    REQUIRE(std::find(list.cbegin(),
                      list.cend(),
                      QString("This is a test string"))!=list.cend());
  }
  {
    QList<int> list;
    int value = 1;
    std::generate_n(std::back_inserter(list),10,[&value](){
      return value++;
    });
    for (auto itr = list.begin();itr != list.end();) {
      if (*itr % 2 == 0) {
        itr = list.erase(itr);
      }else {
        itr++;
      }
    }

    REQUIRE(list.size() == 5);
  }
  {
    QList<int> list{1,2,3,4,5};
    REQUIRE(list.size() == 5);
    QListIterator<int> itr(list);

    int value = 1;
    for (;itr.hasNext();) {
     REQUIRE(itr.next() == value++);
    }
  }
}

