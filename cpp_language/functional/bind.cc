#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std::placeholders;

class Person {
  public:
    Person(const std::string& firstName, const std::string& secondName)
        : _firstName{firstName}, _secondName{secondName} {}

    enum OutputFormat { NameOnly, FullName };

    void print(std::ostream& out, Person::OutputFormat format) const;

  private:
    std::string _firstName;
    std::string _secondName;
    friend void printPerson(const Person&, std::ostream&, Person::OutputFormat);
};

void printPerson(const Person& person, std::ostream& out,
                 Person::OutputFormat format) {
    switch (format) {
    case Person::NameOnly: out << person._firstName << std::endl; break;
    case Person::FullName:
        out << person._firstName << " " << person._secondName << std::endl;
        break;
    }
}
void Person::print(std::ostream& out, Person::OutputFormat format) const {
    switch (format) {
    case Person::NameOnly: out << _firstName << std::endl; break;
    case Person::FullName:
        out << _firstName << " " << _secondName << std::endl;
        break;
    }
}

int main() {
    std::vector<Person> people = {Person("Peter", "Huslter"),
                                  Person("Martha", "Klinton")};
    std::for_each(
        people.cbegin(), people.cend(),
        std::bind(printPerson, _1, std::ref(std::cout), Person::NameOnly));

    std::for_each(people.cbegin(), people.cend(), [](const Person& person) {
        person.print(std::cout,Person::NameOnly);
    });
}