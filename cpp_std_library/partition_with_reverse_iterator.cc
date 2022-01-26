//
// Created by rick on 2022/1/10.
//
#include <iterator>
#include <algorithm>
#include <vector>
#include <fmt/printf.h>

template<typename It>
auto rev(It it) {
  return std::reverse_iterator{it};
}

template<typename InnerIt>
auto unrev(std::reverse_iterator<InnerIt> iterator) {
  return iterator.base();
}

template<typename BidirIt, class Unary>
auto partition(BidirIt first, BidirIt last, Unary p) {
  first = std::find_if_not(first, last, p);

  while (first != last) {
    last = unrev(std::find_if(rev(last), rev(first), p));
    if (first == last) break;
    using std::swap;
    swap(*first++, *--last);
    first = std::find_if_not(first, last, p);
  }
  return first;
}

int main() {
  std::vector<int> v{3, 23, 6, 23, 2, 4, 8, 21, 9};
  auto itr = ::partition(std::begin(v), std::end(v), [](auto v) { return v % 2 == 0; });
  fmt::print("{}", fmt::join(v, ","));

}

