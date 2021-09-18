//
// Created by rick on 2021/9/18.
//
#include <windows.h>
#include <iostream>
int main() {
  SetConsoleOutputCP(CP_UTF8);
  // use implicit markers
  char no_lrm[] = u8"he said \"\u0671\u0679!\" to her"; //
  std::cout << no_lrm << std::endl;
  char lrm[] = u8"he said \"\u0671\u0679!\u200f\" to her"; // LRM U+200F
  std::cout << lrm << std::endl;
}