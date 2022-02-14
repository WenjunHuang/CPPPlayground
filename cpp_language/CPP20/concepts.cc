//
// Created by rick on 2022/2/9.
//

#include <type_traits>
#include <iostream>

template<typename T>
concept IsPointer = std::is_pointer_v<T>;

template<typename T>
T maxValue(T a, T b) {
  return b < a ? a : b;
}

template<IsPointer T>
auto maxValue(T a, T b) {
  return maxValue(*a, *b);
}

int main(int argc, char *argv[]) {
  int x = 42;
  int y = 47;
  std::cout << maxValue(x, y) << '\n';
  std::cout << maxValue(&x, &y) << '\n';

}