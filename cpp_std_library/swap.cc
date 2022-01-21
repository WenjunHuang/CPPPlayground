//
// Created by rick on 2022/1/4.
//
#include <algorithm>
#include <array>
#include <fmt/printf.h>

namespace my {
    class obj {
        int v;
    public:
        obj(int value) : v{value} {}

        void swap(obj &other) {
          using std::swap;
          fmt::print("in obj::swap\n");
          swap(this->v, other.v);
        }
    };

    void swap(obj &a, obj &b) {
      a.swap(b);
    }
}

int main() {
  std::array<int, 3> ar{0, 1, 2};
  std::swap(ar[0], ar[2]);

  fmt::print("{}\n", fmt::join(ar, ","));

  std::vector<my::obj> objs{{1},
                            {2},
                            {3},
                            {4}};
  std::reverse(objs.begin(), objs.end());
//  my::obj m1{1}, m2{2};

}

