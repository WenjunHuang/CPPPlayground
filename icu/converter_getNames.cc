//
// Created by HUANG WEN JUN on 2021/9/19.
//
#include <fmt/printf.h>
#include <unicode/ucnv.h>
int main(){
  int count = ucnv_countAvailable();
  fmt::print("icu contains {} converters\n",count);
  for (int i = 0;i<count ;i++){
    fmt::print("{}\n",ucnv_getAvailableName(i));
  }

}
