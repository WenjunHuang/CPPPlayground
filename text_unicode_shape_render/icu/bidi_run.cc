//
// Created by HUANG WEN JUN on 2021/9/18.
//
#include <fmt/printf.h>
#include <unicode/ubidi.h>
#include <unicode/unistr.h>
#include <iostream>
#include "util.h"

namespace {
using UBiDiPtr = std::unique_ptr<UBiDi, FunctionWrapper<ubidi_close>>;
}

int main() {
  icu::UnicodeString text =
      u"he said \"\u0671\u0679!\u200f\" to her\nthis is the second paragraph";
  int length = text.length();
  UErrorCode status = U_ZERO_ERROR;
  UBiDiPtr ubidi(ubidi_openSized(length, 0, &status));
  if (U_FAILURE(status)) {
    std::cerr << u_errorName(status) << std::endl;
    return -1;
  }

  ubidi_setPara(ubidi.get(), text.getBuffer(), length, UBIDI_DEFAULT_LTR,
                nullptr, &status);
  if (U_FAILURE(status)) {
    std::cerr << u_errorName(status) << std::endl;
    return -1;
  }


  std::string utf8_text;
  text.toUTF8String(utf8_text);
  fmt::print("original text: {}\n", utf8_text);

  {
    auto count = ubidi_countRuns(ubidi.get(), &status);
    std::cout << "total run counts: " << count << std::endl;

    int visual_start, visual_length;
    for (int i = 0; i < count; i++) {
      auto dir =
          ubidi_getVisualRun(ubidi.get(), i, &visual_start, &visual_length);
      auto substr = text.tempSubString(visual_start, visual_length);
      utf8_text.clear();
      substr.toUTF8String(utf8_text);
      fmt::print("{} run, direction:{}, text: {} \n", i + 1, dir, utf8_text);
    }
  }

  {
    auto paragraphs_count = ubidi_countParagraphs(ubidi.get());
    fmt::print("total paragraphs counts: {}\n", paragraphs_count);
    int para_start, para_limit;
    UBiDiLevel para_level;
    for (int i = 0; i < paragraphs_count; i++) {
      ubidi_getParagraphByIndex(ubidi.get(), i, &para_start, &para_limit,
                                &para_level, &status);
      if (U_FAILURE(status)) {
        std::cerr << u_errorName(status) << std::endl;
        break;
      }
      auto para = text.tempSubString(para_start,para_limit);
      utf8_text.clear();
      para.toUTF8String(utf8_text);
      fmt::print("the {} paragraph, bidi level:{} ,text: {}\n",i+1,para_level,utf8_text);
    }
  }

  {
    auto length = ubidi_getResultLength(ubidi.get());
    fmt::print("reordered text length:{}\n",length);
    auto indexMap = std::make_unique<int[]>(length);
    ubidi_getVisualMap(ubidi.get(),indexMap.get(),&status);
    for (int i=0;i<length;i++)
      fmt::print("{}\n",indexMap[i]);
  }
}