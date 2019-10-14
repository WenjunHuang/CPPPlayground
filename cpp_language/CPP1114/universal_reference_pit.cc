#include <iostream>
#include <string>
#include <type_traits>

class Person {
  public:
    template <typename T, typename = std::enable_if_t<
                              !std::is_base_of_v<Person, std::decay_t<T>> &&
                              !std::is_integral_v<T>>>
    explicit Person(T&& n) : name{std::forward<T>(n)} {
        std::cout << "unversal ctor\n";
    }

    explicit Person(int idx) {}

    Person(const Person& other) : name{other.name} {
        std::cout << "copy ctor" << '\n';
    };

  public:
    std::string name;
};

void cloneOfP(Person p) {
    std::cout << "clone of p " << p.name << '\n';
    p.name = "hello world";
}

int main() {
    Person p{"Nancy"};
    Person p1{1};
    cloneOfP(p);
    Person p2(p);
    std::cout << p.name << std::endl;
}