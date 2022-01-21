//
// Created by rick on 2022/1/17.
//
#include <map>
#include <string>

#define DECLARE_GETTER(TYPE,NAME) REMOVE_PAREN(TYPE) get_##NAME()

// Forward to REMOVE_PAREN_HELPER
#define REMOVE_PAREN(A) REMOVE_PAREN_HELPER A
#define REMOVE_PAREN_HELPER(...) __VA_ARGS__
using namespace std;

class Foo {
public:
    DECLARE_GETTER((map<int,string>),Name);
    DECLARE_GETTER(string,Str);
};
int main(){


}

