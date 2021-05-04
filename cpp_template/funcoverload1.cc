//
// Created by rick on 2021/5/3.
//
#include <iostream>
template<typename T>
int f(T)
{
    return 1;
}

template<typename T>
int f(T*)
{
    return 2;
}

template<typename T>
void t(T*,T const* = nullptr,...){
    std::cout << 1 << std::endl;
}

template<typename T>
void t(T const* ,T*,T* = nullptr){
    std::cout << 2 << std::endl;
}

int main(){
    int p = 1;
    t(&p,&p);

}