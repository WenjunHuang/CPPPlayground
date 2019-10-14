#include <iostream>

class WidgetWithInitializerListCtor {
  public:
    WidgetWithInitializerListCtor(int i,bool b){}
    WidgetWithInitializerListCtor(int i,double d){}
    WidgetWithInitializerListCtor(std::initializer_list<long double> il) {
        std::cout << "use initializer_list ctor"<<std::endl;
    }
};

class WidgetWithInitializerListCtorButNotImplicitConversion {
  public:
    WidgetWithInitializerListCtorButNotImplicitConversion(int i,bool b){
        std::cout <<"use (int,bool) ctor" << '\n';
    }
    WidgetWithInitializerListCtorButNotImplicitConversion(int i,double d){
        std::cout <<"use (int,double) ctor" << '\n';
    }
    WidgetWithInitializerListCtorButNotImplicitConversion(std::initializer_list<std::string> il) {
        assert(false);
    }
};

class DefaultCtorAndInitListCtor{
  public:
    DefaultCtorAndInitListCtor(){
        std::cout << "default ctor" << '\n';
    }
    DefaultCtorAndInitListCtor(std::initializer_list<int> il) {
        assert(false);
    }
};
int main(){
    double x,y,z;
//    int sum1{x+y+z}; // brace initialization doesn't allow implicit narrowing conversion
    int sum2(x+y+z); // ok with parentheses
    int sum3 = x + y + z; // also ok with = initialization

    WidgetWithInitializerListCtor w1{1,true}; // calls initializer_list ctor 1 and true are converted to long double
    WidgetWithInitializerListCtorButNotImplicitConversion w2 {1,true};
    WidgetWithInitializerListCtorButNotImplicitConversion w3 {1,1.0};

    DefaultCtorAndInitListCtor d{};
    DefaultCtorAndInitListCtor d2(); // declare a function!!!!
}