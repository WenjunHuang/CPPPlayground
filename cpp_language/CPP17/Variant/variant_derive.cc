//
// Created by rick on 2019/11/7.
//
#include <string>
#include <variant>
#include <iostream>

class Derived : public std::variant<int, std::string> {
};

int main() {
  Derived d = {{"hello"}};
  std::cout << d.index() << '\n';
  std::cout << std::get<1>(d) << '\n';
  d.emplace<0>(77);
  std::cout << std::get<0>(d) << '\n';

  if (auto ptr = std::get_if<0>(&d)) {
    std::cout << "0" << '\n';
  }

  if (auto ptr = std::get_if<int>(&d)) {
    std::cout << "int" << '\n';
  }

  try {
    auto s = std::get<std::string>(d);
  } catch (const std::bad_variant_access &e) {
    std::cout << "Exception: " << e.what() << '\n';
  }
}