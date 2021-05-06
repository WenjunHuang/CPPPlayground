//
// Created by rick on 2021/5/4.
//
#include <iostream>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

class Person{
public:
    string firstName() const {
       return _firstName;
    }
    string lastName() const {
        return _lastName;
    }
private:
    string _firstName;
    string _lastName;
};

class PersonSortCriterion {
public:
    bool operator()(const Person& p1,const Person& p2) const {
        return p1.lastName() < p2.lastName() ||
            (p1.lastName() == p2.lastName() && p1.firstName() < p2.firstName());
    }
};

int main(){
    set<Person,PersonSortCriterion> coll;

}