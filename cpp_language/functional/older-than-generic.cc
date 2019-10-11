//
// Created by rick on 2019/10/11.
//

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
class older_than {
  public:
    older_than(int limit) : _limit(limit) {}

    template <typename T> bool operator()(T&& object) const {
        return std::forward<T>(object).age() > _limit;
    }

  private:
    int _limit;
};

class Person {
  public:
    Person(int age) : _age(age) {}

    int age() const { return _age; }

  private:
    int _age;
};

class Car {
  public:
    Car(int age) : _age(age) {}

    int age() const { return _age; }

  private:
    int _age;
};

int main() {
//    older_than predicate(5);
    auto predicate = [limit = 5](auto&& obj){
        return obj.age() > limit;
    };
    std::vector<Person> persons{Person(55),Person(66),Person(12),Person(23)};
    std::vector<Car> cars{Car(55),Car(66),Car(12),Car(23)};
    auto result = std::count_if(persons.begin(),persons.end(),predicate);
    std::cout << result << std::endl;
    result = std::count_if(cars.begin(),cars.end(),predicate);
    std::cout << result << std::endl;
}
