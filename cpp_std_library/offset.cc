//
// Created by rick on 2022/1/7.
//
#include <fmt/printf.h>
#include <cstdio>

struct Foo {
    int16_t a; // 2
    int8_t b; // 1 byte but pad to 2 bytes
    char32_t c[100]; // 100 * 4 = 400
    uint32_t d; // 4
};

int main() {
  fmt::print("size of Foo is  {} bytes.\n", sizeof(Foo));
  fmt::print("offset of d of Foo is {} bytes", offsetof(Foo, d));

}