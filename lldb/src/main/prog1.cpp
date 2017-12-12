//
// Created by xxzyjy on 04/12/2017.
//
#include <iostream>
#include <vector>

using namespace std;

void my_subroutine(){
  cout << "Hello world" << endl;
}

class MyClass {
private:
  std::string _name;
public:
  MyClass(const std::string & name);
  void foo();
};

MyClass::MyClass(const std::string)

void MyClass::foo() {
  std::vector<int> col{1,2,3,4,5};
  std::for_each(col.cbegin(),col.cend(),[=](auto element){
    std::cout << element << " ";
  });
}

int main() {

  MyClass my;
  my.foo();
  return  0;
}
