//
// Created by HUANG WEN JUN on 2021/12/25.
//
#include <list>
#include <forward_list>
#include <iterator>
#include <algorithm>
#include <set>
#include <iostream>

int main() {
    int a[]{1, 2, 3, 4, 5};
    std::list<int> lst{1, 2, 3, 4, 5};
    std::forward_list<int> flst{1, 2, 3, 4, 5};

    assert(std::distance(std::begin(a), std::end(a)) == 5);
    assert(std::distance(std::end(a), std::begin(a)) == -5);

    assert(std::distance(std::begin(lst), std::end(lst)) == 5);
    assert(std::distance(std::begin(flst), std::end(flst)) == 5);

    std::set<int> s{1, 2, 3, 10, 32, 99};
    bool present;
    //O(n)
    present = std::count(s.begin(), s.end(), 32);
    // O(logn)
    present = s.count(42);

    assert(std::find_if(lst.begin(), lst.end(), [](auto &&v) {
        if (v % 2 == 0)
            return true;
        else
            return false;
    }) != lst.end());

    assert(std::all_of(std::begin(a), std::end(a), [](auto &&v) {
        return v > 0;
    }));

    assert(std::any_of(std::begin(a), std::end(a), [](auto &&v) {
        return v < 0;
    }) == false);
    assert(std::none_of(std::begin(a), std::end(a), [](auto &&v) {
        return v < 0;
    }) == true);

//    std::istream_iterator<char> ii(std::cin);
//    std::istream_iterator<char> iend{};
//    std::string str{"hello"};
//    std::find_first_of(ii,iend,str.begin(),str.end());

    assert(std::equal(std::begin(a), std::end(a), lst.begin(), lst.end()));
    auto m = std::mismatch(lst.begin(),lst.end(),
                  flst.begin(),flst.end());
    assert(m == std::make_pair(lst.end(), flst.end()));
}