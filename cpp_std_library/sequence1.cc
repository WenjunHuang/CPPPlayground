//
// Created by rick on 2021/5/4.
//
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>

using namespace std;

class IntSequence {
private:
    int value;
public:
    IntSequence(int initialValue):value{initialValue}{}

    int operator()(){
        return ++value;
    }
};

int main() {
    list<int> coll;

    generate_n(back_inserter(coll),
        9,
        IntSequence(1));

    for (const auto& item:coll){
        cout << item << endl;
    }

    // replace second to last element but one with values starting at 42
    generate(next(coll.begin()),
        prev(coll.end()),
        IntSequence(42));

    for (const auto& item:coll){
        cout << item << endl;
    }
}
