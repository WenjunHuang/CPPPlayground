//
// Created by rick on 2021/5/7.
//
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <random>
#include <utility>

class Base {
public:
    virtual ~Base() {}
};

class Customer: public Base{
private:
    std::string name;
    std::vector<int> values;
public:
    Customer(const std::string& n):name{n}{
        assert(!name.empty());
    }

    std::string getName() const {return name;}
    void addValue(int val){values.push_back(val);}

    friend std::ostream& operator<<(std::ostream& strm,const Customer& c){
        strm << '[' << c.name << ": ";
        for (int val: c.values){
            strm << val << ' ';
        }
        strm << ']';
        return strm;
    }
};

Customer createCustomer() {
    Customer c{"Wenjun"};
    c.addValue(10);
    c.addValue(100);
    return c; // uses move semantics if not optimized away
}
int main(){
    Customer c{"Wolfgang Amadeus Mozart"};
    for(int val:{0,8,15}){
        c.addValue(val);
    }
    std::cout << "c: " << c << '\n';

    // insert the customer twice into a collection of customers:
    std::vector<Customer> customers;
    customers.push_back(c); // copy into the vector
    customers.push_back(std::move(c)); // move into the vector
    customers.push_back(createCustomer()); //
    std::cout << "c: " << c << '\n'; //print value of moved-from customer

    // print all customers in the collections:
    std::cout << "customers:\n";
    for (const Customer& cust:customers) {
        std::cout << " " << cust << '\n';
    }
}