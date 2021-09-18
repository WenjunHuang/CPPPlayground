//
// Created by rick on 2021/9/18.
//

#include <fmt/format.h>
#include <unicode/unistr.h>
#include <windows.h>
#include <iostream>

int main() {
  SetConsoleOutputCP(CP_UTF8);
  char16_t str[] = u"这是一串utf16字符串";
  icu::UnicodeString unicodeString(str,sizeof(str)/sizeof(str[0]));
  std::string utf8String;
  unicodeString.toUTF8String(utf8String);

  std::cout << utf8String << std::endl;
}
