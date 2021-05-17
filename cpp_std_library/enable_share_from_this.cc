//
// Created by rick on 2021/5/4.
//
#include <memory>
#include <iostream>

using namespace std;
struct Good:enable_shared_from_this<Good>{
    shared_ptr<Good> getptr() {
        return shared_from_this();
    }
};

struct Bad{
    shared_ptr<Bad> getptr() {
        return shared_ptr<Bad>(this);
    }

    ~Bad() {
        cout << "Bad::~Bad() called \n";
    }
};

int main() {
    shared_ptr<Good> gp1 = make_shared<Good>();
    shared_ptr<Good> gp2 = gp1->getptr();
    cout << "gp2.use_count() = "<< gp2.use_count() << endl;

    // Bad: shared_from_this is called without having std::shared_ptr owing the caller
    try {
        Good not_so_good;
        shared_ptr<Good> gp1 = not_so_good.getptr();
    }catch(std::bad_weak_ptr& e){
        std::cout << e.what() << '\n';
    }
}