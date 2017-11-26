//
// Created by xxzyjy on 26/10/2017.
//
#include <QStringList>
#include <QVariant>
#include"catch.hpp"

TEST_CASE("qvariant_opts","[QVariant]"){
  {
    QStringList s1{"A","B","C","D"};
    QVariant slv(s1);

    REQUIRE(slv.type() == QVariant::StringList);
  }
}

