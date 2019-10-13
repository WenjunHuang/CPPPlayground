#include <complex>
#include <iostream>
#include <string>
struct Data {
    std::string name;
    double value;
};

struct MoreData : Data {
    bool done;
};

template<typename T>
struct D: std::string,std::complex<T> {
    std::string data;
};


int main() {
    MoreData x{{"test1", 6.778}, false};
    MoreData y{"test1", 6.772, false};

    D<double> s{{"hello"},{4.5,6.7},"world"};
    std::cout << s.data;
}
