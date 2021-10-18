//
// Created by rick on 2021/10/18.
//
#include <skia/core/SkFontMgr.h>
#include <skia/core/SkTypeface.h>
#include <fmt/printf.h>
int main() {
  auto fontMgr = SkFontMgr::RefDefault();
  sk_sp<SkFontStyleSet> fontStyleSet(fontMgr->matchFamily("Segoe UI"));
  if (fontStyleSet != nullptr){
    fmt::print("count: {}",fontStyleSet->count());
    for (size_t i = 0;i<fontStyleSet->count();++i){
      sk_sp<SkTypeface> typeface(fontStyleSet->createTypeface(i));
    }
  }
}