//
// Created by rick on 2021/5/4.
//
#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <QVector>

using namespace std;

class IntSequence {
private:
    int value;

public:
    IntSequence(int initialValue)
        : value { initialValue }
    {
    }

    int operator()()
    {
        return ++value;
    }
};

int main()
{
    QVector<int> coll;
    IntSequence seq { 1 };

    // insert values from 1 to 4
    // - pass function object by reference so that it will continue with 5
    generate_n<back_insert_iterator<QVector<int>>,
        int,
        IntSequence&>(back_inserter(coll),
        4,
        seq);

    for (const auto& item : coll) {
        cout << item << ' ';
    }

    // insert values from 42 to 45
    generate_n(back_inserter(coll),
        4,
        IntSequence(42));

    cout << '\n';
    for (const auto& item : coll) {
        cout << item << ' ';
    }

    // continue with first sequence
    // - pass function object by value so that it will continue with 5 again
    generate_n(back_inserter(coll),
        4,
        seq);
    cout << '\n';
    for (const auto& item : coll) {
        cout << item << ' ';
    }

    // continue with first sequence again
    generate_n(back_inserter(coll),
        4,
        seq);
    cout << '\n';
    for (const auto& item : coll) {
        cout << item << ' ';
    }
}
