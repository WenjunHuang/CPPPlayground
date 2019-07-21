//
// Created by rick on 18-8-18.
//
#define CATCH_CONFIG_MAIN
#include "catch.h"

#include <clang/Tooling/Tooling.h>
#include "FindNamedClassAction.h"

TEST_CASE("runToolOnCode","[CanSyntaxCheckCode]"){
  using namespace clang::tooling;

  runToolOnCode(new FindNamedClassAction,"namespace n { namespace m { class C {}; } }");
}

