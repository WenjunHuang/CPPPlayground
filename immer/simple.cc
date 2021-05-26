//
// Created by rick on 2021/5/25.
//
#include <cassert>
#include <immer/vector.hpp>

int main()
{
    immer::vector<int> v0{};
    const auto v1 = v0.push_back(13);
    assert(v0.size() == 0 && v1.size() == 1 && v1[0] == 13);
}