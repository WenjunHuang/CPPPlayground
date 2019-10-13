
#include <cstddef>
#include <tuple>
#include <type_traits>

class MyClass {
  public:
    int GetA() const { return a; }
    float GetB() const { return b; }

  private:
    int a;
    float b;
};

template <std::size_t I> auto get(MyClass& c) {
    if constexpr (I == 0)
        return c.GetA();
    else if constexpr (I == 1)
        return c.GetB();
}

namespace std {
    template <> struct tuple_size<MyClass> : integral_constant<size_t, 2> {};
    template <> struct tuple_element<0, MyClass> { using type = int; };
    template <> struct tuple_element<1, MyClass> { using type = float; };
} // namespace std

struct Base {
    Base& operator()(int i) { return *this; }
    Base& operator()(int a, int b) { return *this; }
};

struct Derived : Base {
    using Base::operator();
};

int main() {
    Derived d;
    d(100);
    d(100, 200);
}