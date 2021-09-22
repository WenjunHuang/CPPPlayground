//
// Created by rick on 2021/9/22.
//
#include <skia/core/SkString.h>
#include <skia/core/SkFontMgr.h>
#include <skia/core/SkTypeface.h>
#include <unicode/unistr.h>
#include <cassert>
#include <fmt/printf.h>

int main(){
  auto fontMgr = SkFontMgr::RefDefault();
  SkFontStyle style = SkFontStyle::Normal();
  const char* languages[] = {"zh"};
  char16_t utf16[] = u"\U0001F600";
  UChar32 uchar;
  int i = 0;
  U16_NEXT_OR_FFFD(utf16,i,1,uchar);
  auto typeFace = fontMgr->matchFamilyStyleCharacter(nullptr,style,languages,1,uchar);
  assert(typeFace);
  SkString familyName;
  typeFace->getFamilyName(&familyName);
  fmt::print("{}",familyName.c_str());

}