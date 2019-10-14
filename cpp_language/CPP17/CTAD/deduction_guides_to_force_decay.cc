#include <iostream>
#include <type_traits>

template<typename T>
struct C {
    C(const T& v){
        if (std::is_pointer_v<T>)
            std::cout << "T is a pointer" << std::endl;
    }
};

template<typename T>
struct D {
    D(const T& v){
        if (std::is_pointer_v<T>)
            std::cout << "T is a pointer" << std::endl;
    }
};

template<typename T>D(T) -> D<T>;

int main(){
    C c{"abc"}; // T is char[3] because of reference
    D d{"abc"}; // with custom deduction rule,T is correctly decay as pointer

}