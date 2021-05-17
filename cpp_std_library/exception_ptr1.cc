//
// Created by rick on 2021/5/4.
//
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

void handleEptr(std::exception_ptr eptr){
    try {
        if (eptr)
            std::rethrow_exception(eptr);
    }catch(const std::exception& e){
        std::cout << "Caught exception \"" << e.what() << "\"\n";
    }
}

int main() {
    std::exception_ptr eptr;
    try {
        std::string().at(1);
    }catch(...){
        eptr = std::current_exception();
    }
    handleEptr(eptr);
}