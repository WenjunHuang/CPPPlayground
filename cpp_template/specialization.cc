//
// Created by rick on 2020/11/14.
//
#include <iostream>

template <class T = void, class SourceOperator = void> class observable;

template <class T, class SourceOperator> class observable {
  public:
    observable() {}
};

template <> class observable<void, void> {
  public:
    template <class T> static T Add(T a, T b) { return a + b; }
};

int main(int argc, char* argv[]) {
    std::cout << observable<>::Add(1, 2) << std::endl;
}