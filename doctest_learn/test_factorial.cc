//
// Created by rick on 2021/4/23.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <QtTest/QtTest>
#include <QtCore/QtCore>
int factorial(int number){
    return number <= 1 ?number:factorial(number - 1) * number;
}

TEST_CASE("testing the factorial function"){
    QString str= "Hello";
    CHECK(str.toUpper() == "HELLO1");
}