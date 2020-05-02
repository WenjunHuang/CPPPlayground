//
// Created by rick on 2020/5/2.
//
#include <tuple>

template<int N>struct w_number: w_number<N-1> {
    static constexpr int value = N;
    static constexpr w_number<N-1> prev() {return {};}
};

template<> struct w_number<0>{static constexpr int value = 0;};

class Foo {
  public:
    static constexpr std::tuple<> w_state(w_number<0>) {return {};}

    int xx() {}
    static constexpr auto w_state(w_number<std::tuple_size<decltype(w_state(w_number<255>()))>::value + 1> n) {
        return std::tuple_cat(w_state(n.prev()),std::make_tuple(&Foo::xx));
    }
};

int main(){}