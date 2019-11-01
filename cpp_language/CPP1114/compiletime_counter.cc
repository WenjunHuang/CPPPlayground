#include <iostream>
#include <tuple>
using namespace std;

template <int N> struct w_number : public w_number<N - 1> {
    static constexpr int value = N;
    static constexpr w_number<N - 1> prev() { return {}; }
};

// Specialize for 0 to break the recursion
template <> struct w_number<0> { static constexpr int value = 0; };

class Foo {
  public:
    static constexpr tuple<> w_state(w_number<0>) { return {}; }

    static constexpr auto
    w_state(w_number<tuple_size_v<decltype(w_state(w_number<255>()))> + 1> n)
        -> decltype(tuple_cat(w_state(n.prev()), make_tuple(1))) {
        return tuple_cat(w_state(n.prev()), make_tuple(1));
    }

    static constexpr auto
    w_state(w_number<tuple_size_v<decltype(w_state(w_number<255>()))> + 1> n)
        -> decltype(tuple_cat(w_state(n.prev()), make_tuple(2))) {
        return tuple_cat(w_state(n.prev()), make_tuple(2));
    }
};
template <int N, typename... Args>
enable_if_t<N == 0, std::ostream&> printTuple(std::ostream& out,
                                              const tuple<Args...>& tuple) {
    return out;
}

template <int N, typename... Args>
enable_if_t<N != 0, std::ostream&> printTuple(std::ostream& out,
                                              const tuple<Args...>& tuple) {
    printTuple<N - 1, Args...>(out, tuple);
    out << get<N - 1>(tuple) << '\n';
    return out;
}

template <typename... Args>
std::ostream& operator<<(std::ostream& out, const tuple<Args...>& tuple) {
    return printTuple<sizeof...(Args)>(out, tuple);
}

int main() {
    auto result = Foo::w_state(w_number<255>());
    cout << result;
}