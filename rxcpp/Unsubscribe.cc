//
// Created by HUANG WEN JUN on 2020/11/17.
//

#include <rx.hpp>
int main() {
    auto subs = rxcpp::composite_subscription();
    auto values = rxcpp::observable<>::range(1,12);
    values.subscribe(subs,[&subs](int v){
                               printf("OnNext: %d\n",v);
                               if (v == 6)
                                   subs.unsubscribe();
                           },[](){printf("OnCompleted\n");});
}