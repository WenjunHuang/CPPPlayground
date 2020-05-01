#include <rx.hpp>
#include <iostream>
int main() {
    rxcpp::observable<>::range(5,10)
    .subscribe([](auto value){
        std::cout << "Received:" << value << std::endl;
    });
}