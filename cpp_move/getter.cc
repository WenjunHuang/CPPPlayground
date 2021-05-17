//
// Created by rick on 2021/5/8.
//
#include <iostream>
#include <string>
#include <vector>

class Person {
private:
    std::string name;

public:
    Person(std::string n)
        : name { std::move(n) }
    {
    }

    std::string getName() &&
    {
        return std::move(name);
    }

    const std::string& getName() const&
    {
        return name;
    }
};

void foo()
{
    Person p { "abc" };
    std::vector<std::string> col;
    col.push_back(p.getName()); // calls getName() const&
    col.push_back(std::move(p).getName()); // calls getName() && (OK,p no longer used)
}

Person returnPersonByValue()
{
    return Person { "Hello" };
}

int main()
{
    Person p { "Ben" };
    std::cout << p.getName(); // fast(returns reference)
    std::cout << returnPersonByValue().getName(); // fast(use move())

    std::vector<Person> coll;
    coll.push_back(Person { "A" });
    coll.push_back(Person { "B" });
    for (const auto& person : coll) {
        if (person.getName().empty()) { // fast (returns reference)
            std::cout << "found empty name\n";
        }
    }

    for (char c : returnPersonByValue().getName()) { // safe and fast(uses move())
        if (c == ' ') { }
    }
}