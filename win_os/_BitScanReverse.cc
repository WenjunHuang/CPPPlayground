//
// Created by rick on 2021/11/2.
//
#include <fmt/printf.h>
#include <intrin.h>
#include <iostream>
#pragma intrinsic(_BitScanReverse)
int main(int argc, char* argv[]) {
  uint32_t mask = 0x1000;
  unsigned long index;

  fmt::print("Enter a positive integer as the mask:\n");
  std::cin >> mask;
  auto isNonzero = _BitScanReverse(&index, mask);
  if (isNonzero) {
    fmt::print("Mask: {} Index: {}\n", mask, index);
  } else {
    fmt::print("No set bits found. Mask is zero.\n");
  }
}