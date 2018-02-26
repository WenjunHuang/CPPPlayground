//
// Created by xxzyjy on 10/12/2017.
//
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
void say_hello() {
  std::cout << "Hello from cpp" << std::endl;
}
#ifdef __cplusplus
};
#endif
