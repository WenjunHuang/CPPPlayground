//
// Created by rick on 2021/9/18.
//
#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>
int main() {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
  // use implicit markers
  char no_lrm[] = u8"he said \"\u0671\u0679!\" to her";  //
  std::cout << no_lrm << std::endl;
  char lrm[] = u8"he said \"\u0671\u0679!\u200f\" to her";  // LRM U+200F
  std::cout << lrm << std::endl;
}