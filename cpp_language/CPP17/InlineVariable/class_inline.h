#include <string>
class MyClass{
  public:
    inline static std::string msg{"OK"};

    static MyClass max;
};

inline MyClass MyClass::max;

void printIn0();
void printIn1();