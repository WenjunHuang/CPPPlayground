//
// Created by HUANG WEN JUN on 2021/9/21.
//
#include <fmt/printf.h>
#include <unicode/ustring.h>
#include <codecvt>
#include <iostream>
#include <memory>

int main() {
  char utf8[] = u8"你好世界，\U0001F600\u0671\u0679哈哈";
  size_t length = sizeof(utf8) / sizeof(utf8[0]);

  fmt::print("origin utf8:{}\n", utf8);

  {
    auto utf16 = std::make_unique<char16_t[]>(length + 1);
    UErrorCode status = U_ZERO_ERROR;
    u_strFromUTF8(utf16.get(), length + 1,nullptr, utf8, -1, &status);
    if (U_FAILURE(status)) {
      fmt::print("error {}", u_errorName(status));
      return -1;
    }

    // use std lib to convert utf-16 to utf-8
    std::string utf8Result =
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}
            .to_bytes(utf16.get());
    std::cout << utf8Result << std::endl;
  }
}