//
// Created by rick on 2019/11/8.
//
#include <variant>
#include <iostream>
#include <type_traits>
#include <vector>

template<typename T> using RawType = std::remove_const_t<std::remove_reference_t<T>>;
template<typename T, typename U> inline constexpr bool EqualType = std::is_same_v<RawType<T>, U>;

template<typename ... Ts>
struct overload : Ts ... {
    using Ts::operator() ...;
};

template<typename...Ts>
overload(Ts...) -> overload<Ts...>;

int main() {
  int i = 0;
  int s = 0;
  auto printvariant = [&](const auto &val) {
      if constexpr (std::is_convertible_v<decltype(val), int>) {
        std::cout << "int " << val << std::endl;
        i++;
      } else if (EqualType<decltype(val), std::string>) {
        std::cout << "string " << val << std::endl;
        s++;
      }
  };
  std::variant<int, std::string> var;
  std::visit(printvariant, var);
  var = "hello world";
  std::visit(printvariant, var);

  std::cout << "int count:" << i << '\n';
  std::cout << "string count:" << s << '\n';

  using IntOrDouble = std::variant<int, double>;
  std::vector<IntOrDouble> coll{42, 7.7, 0, -0.7};
  double sum{0};
  for (const auto &elem:coll) {
    sum += std::visit([](const auto &val) -> double {
        return val;
    }, elem);
  }
  std::cout << sum << '\n';

  using ResultType = std::variant<int,double>;
  std::variant<int,double> result = std::visit(overload{
    [](std::string &s) { std::cout << "string:" << s << '\n';return ResultType{(int)s.size()}; },
    [](auto& i) { i *= 2; return ResultType{10.0};}
  }, var);

  //std::cout << result << std::endl;
}
