//
// Created by rick on 2017/7/25.
//

#include <boost/variant.hpp>
#include <string>
#include <iostream>
#include <string>
#include "gtest/gtest.h"

using namespace boost;
using namespace std;

struct output : public static_visitor<> {
  void operator()(double d) const { cout << d << endl; }

  void operator()(char c) const { cout << c << endl; }

  void operator()(string s) const { cout << s << endl; }
};

struct static_output:public static_visitor<>{
  template<typename T>void operator()(T t) const {
    cout << t << endl;
  }
};

TEST(Variant, AccessingValueWithGet) {
  variant<double, char, string> v;
  v = 3.14;
  cout << get<double>(v) << endl;

  v = 'A';
  cout << get<char>(v) << endl;

  v = "Boost";
  cout << get<string>(v) << endl;
}

TEST(Variant, DirectOutput) {
  variant<double, char, string> v;
  v = 3.14;
  cout << v << endl;

  v = 'A';
  cout << v << endl;

  v = "Boost";
  cout << v << endl;
}

TEST(Variant, FunctionTemplate) {
  variant<double, char, string> v;
  v = 3.14;
  apply_visitor(static_output{}, v);

  v = 'A';
  apply_visitor(static_output{}, v);

  v = "Boost";
  apply_visitor(static_output{}, v);
}


TEST(Variant, V1) {
  variant<double, char, string> v;
  v = 3.14;
  apply_visitor(output{}, v);
  v = 'A';
  apply_visitor(output{}, v);
  v = "Boost";
  apply_visitor(output{}, v);
}