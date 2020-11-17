//
// Created by HUANG WEN JUN on 2020/11/17.
//

#include <rx.hpp>
int main() {
    auto values = rxcpp::observable<>::range(1);
    auto s1     = values.take(3).map([](int prime) { return 2 * prime; });
    auto s2     = values.take(3).map([](int prime) { return prime * prime; });

    s1.concat(s2).subscribe([](int p) { printf(" %d\n", p); },
                            []() { printf("OnCompleted\n"); });
}