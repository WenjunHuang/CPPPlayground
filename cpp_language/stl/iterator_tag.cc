//
// Created by rick on 2020/2/23.
//
#include <vector>
#include <iostream>

void foo(std::bidirectional_iterator_tag t [[maybe_unused]]) {
    std::cout << "std::vector's iterators are indeed bidirectional...\n";
}

void bar(std::random_access_iterator_tag){
    std::cout << "... and random-access,too!";
}

void bar(std::forward_iterator_tag){
    std::cout << "forward_iterator_tag is not as goo as a match.";
}

int main() {
    using It = std::vector<int>::iterator;
    using ItCat = std::iterator_traits<It>::iterator_category;
    using IntPtrCat = std::iterator_traits<int*>::iterator_category;
    foo(ItCat{});
    bar(ItCat{});

}