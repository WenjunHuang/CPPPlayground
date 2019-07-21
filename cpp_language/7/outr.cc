#include <iostream>
#include <type_traits>
template <typename T> void outR(T& arg) {
    static_assert(!std::is_const_v<T>, "out parameter of foo<T>(T&) is const");
    if (std::is_array<T>::value) {
        std::cout << "got array of " << std::extent<T>::value << " elems\n";
    }
}

template <typename T, typename = std::enable_if_t<!std::is_const_v<T>>>
void outR2(T& arg) {}

int main() {
    int arr[4];
    outR(arr);

    std::string sname{"sname"};
    outR(sname);
    outR2(sname);

    std::string const cname{"cname"};
    outR2(cname);
}
