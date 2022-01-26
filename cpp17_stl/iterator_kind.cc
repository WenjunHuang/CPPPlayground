//
// Created by HUANG WEN JUN on 2021/12/25.
//

#include <iostream>
#include <vector>
#include <iterator>

void foo(std::bidirectional_iterator_tag t[[maybe_unused]]){
    std::cout <<"std::vector's iterators are indeed bidirectional..";
}

void bar(std::random_access_iterator_tag) {
    std::cout <<"... and random-access, too!";
}
void bar(std::forward_iterator_tag){
    std::cout <<"forward_iterator_tag is not as good a match";
}
int main(){
    using It = std::vector<int>::iterator;
    foo(std::iterator_traits<It>::iterator_category{});
    bar(It::iterator_category{});


}

