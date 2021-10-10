//
// Created by rick on 2021/9/22.
//

#pragma once
#include <harfbuzz/hb.h>
#include "shaper.h"
#include "unicode.h"

using UnicodeScript = std::unique_ptr<ScriptIterator>;
class UnicodeScriptRunIterator final : public Shaper::ScriptRunIterator {
 public:
  UnicodeScriptRunIterator(UnicodeScript, const char* utf8, size_t utf8Bytes)
      : fCurrent(utf8),
        fBegin(utf8),
        fEnd(fCurrent + utf8Bytes),
        fCurrentScript(HB_SCRIPT_UNKNOWN) {}

  hb_script_t hb_script_for_unichar(SkUnichar u) {
    return hb_unicode_script(hb_unicode_funcs_get_default(), u);
  }
  void consume() override {
    SkASSERT(fCurrent < fEnd);
    SkUnichar u;
    int i = 0;
    U8_NEXT_OR_FFFD(fCurrent, i, fEnd - fCurrent, u);
    fCurrent += i;
    fCurrentScript = hb_script_for_unichar(u);
    while(fCurrent < fEnd) {
      const char* prev = fCurrent;
      i = 0;
      U8_NEXT_OR_FFFD(fCurrent, i, fEnd - fCurrent, u);
      fCurrent += i;
      auto script = hb_script_for_unichar(u);
      if (script != fCurrentScript) {
        if (fCurrentScript == HB_SCRIPT_INHERITED || fCurrentScript == HB_SCRIPT_COMMON) {
          fCurrentScript = script;
        } else if (script == HB_SCRIPT_INHERITED || script == HB_SCRIPT_COMMON){
          continue;
        } else {
          fCurrent = prev;
          break;
        }
      }
    }

    if (fCurrentScript == HB_SCRIPT_INHERITED) {
      fCurrentScript = HB_SCRIPT_COMMON;
    }
  }
  size_t endOfCurrentRun() const override {return fCurrent - fBegin;}
  bool atEnd() const override {return fCurrent == fEnd;}
  SkFourByteTag currentScript() const override {return SkSetFourByteTag(HB_UNTAG(fCurrentScript));}

 private:
  const char* fCurrent;
  const char* const fBegin;
  const char* const fEnd;
  hb_script_t fCurrentScript;
};