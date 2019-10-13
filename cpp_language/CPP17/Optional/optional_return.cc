#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

std::optional<std::vector<int>> CreateVec() {
    std::vector<int> v{0, 1, 2, 3, 4};
    std::cout << std::hex << v.data() << '\n';
    return v;
}

std::optional<std::string> CreateStr() {
    std::string s{"Hello Super Long String"};
    std::cout << std::hex << static_cast<void*>(s.data()) << '\n';
    return {s}; // this one will cause a copy
    //return s; // this one moves
}

std::unique_ptr<int> foo() {
    std::unique_ptr<int> p;
    // return {p}; // uses copy of unique_ptr and so it breaks...
    return p; // this one moves, so it's fine with unique_ptr
}

int main() {
    auto v = CreateVec();
    std::cout << std::hex << v->data() << '\n';

    auto s = CreateStr();
}