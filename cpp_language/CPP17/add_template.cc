#include <iostream>
#include <type_traits>
template <class T, class U>
auto add(const T& a, const U& b) -> decltype(a + b) {
    return a + b;
}

template <class T> struct is_int { static bool const value{false}; };

template <> struct is_int<int> { static bool const value{true}; };

template <class T> constexpr bool is_int_v{is_int<T>::value};

template<class T,class U>struct is_same {
    static const bool value{false};
};

template<class T>struct is_same<T,T>{
    static const bool value{true};
};

template<class T,class U>constexpr bool is_same_v{is_same<T,U>::value};

template <class T>struct remove_const{
    using type = T;
};

template<class T>struct remove_const<const T>{
    using type = T;
};

template<class T>using remove_const_t = typename remove_const<T>::type;

struct Foo{
    int a;
    int b;
    std::string name;
};

int main(int argc, char* argv[]) {
    std::cout << add(100, 100.1) << std::endl;
    std::cout << std::boolalpha << "float: " << is_int<float>::value << std::endl;
    std::cout << std::boolalpha << "double: " << is_int_v<double> << std::endl;

    std::cout << is_same_v<int,remove_const_t<const int>> << '\n';
    std::cout << is_same_v<int const,remove_const_t<const int>> << '\n';

    std::cout << std::is_pod_v<Foo> << '\n';
    std::cout << std::is_trivially_constructible_v<Foo> << '\n';

}