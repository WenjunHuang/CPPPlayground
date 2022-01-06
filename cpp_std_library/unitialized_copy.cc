//
// Created by rick on 2022/1/4.
//
#include <vector>
#include <algorithm>
#include <memory>
#include <cassert>
#include <string>

int main() {
  alignas(std::string) char b[5 * sizeof(std::string)]{};
  std::string *sb = reinterpret_cast<std::string *>(b);

  std::vector<const char *> vec = {"quick", "brown", "fox"};

  auto end = std::uninitialized_copy(vec.begin(), vec.end(), sb);

  assert(end == sb + 3);

  std::destroy(sb, end);

}