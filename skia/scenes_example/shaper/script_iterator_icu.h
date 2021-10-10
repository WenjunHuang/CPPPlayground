//
// Created by HUANG WEN JUN on 2021/9/20.
//

#ifndef CPPPLAYGROUND_SCRIPT_ITERATOR_ICU_H
#define CPPPLAYGROUND_SCRIPT_ITERATOR_ICU_H
#include "unicode.h"
class ScriptIterator_icu : public ScriptIterator {
 public:
  bool getScript(SkUnichar u, ScriptID* script) override {
    UErrorCode status = U_ZERO_ERROR;
    UScriptCode scriptCode = uscript_getScript(u, &status);
    if (U_FAILURE(status)) {
      return false;
    }
    if (script) {
      *script = (ScriptID)scriptCode;
    }
    return true;
  }

  static std::unique_ptr<ScriptIterator> makeScriptIterator() {
    return std::unique_ptr<ScriptIterator>(new ScriptIterator_icu());
  }
};

#endif  // CPPPLAYGROUND_SCRIPT_ITERATOR_ICU_H
