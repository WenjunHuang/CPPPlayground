//
// Created by HUANG WEN JUN on 2021/9/21.
//
#include <fmt/printf.h>
#include <unicode/ustring.h>
#include <memory>

int main() {
  char16_t utf16[] = u"你好世界，\U0001F600 \u0671\u0679哈哈";

  UErrorCode status = U_ZERO_ERROR;
  int32_t length = 0;
  u_strToUTF8(nullptr, 0, &length, (const UChar*)utf16,
              -1, &status);
  if (U_FAILURE(status)) {
    fmt::print("error {}, dest length:{}\n", u_errorName(status), length);
  }

  auto dest = std::make_unique<char[]>(length+1);
  status = U_ZERO_ERROR;
  u_strToUTF8(dest.get(), length+1 , nullptr, (const UChar*)utf16,
              -1, &status);
  if (U_FAILURE(status)) {
    fmt::print("error again {}, dest length:{}\n", u_errorName(status), length);
  }

  fmt::print("{}",dest.get());
}