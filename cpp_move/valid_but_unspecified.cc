//
// Created by rick on 2021/5/7.
//
#include <string>
#include <vector>
#include <utility>
#include <fmt/format.h>

int main() {
    std::string s{"Hello World"};
    std::vector<std::string> coll;
    coll.push_back(std::move(s));

    fmt::print("{}\n",s);
    fmt::print("{}\n",s.size());
//    fmt::print("{}\n",s[0]); // ERROR(ud)
//    fmt::print("{}\n",s.front()); // ERROR(ud)

    s = "new value"; //ok

}