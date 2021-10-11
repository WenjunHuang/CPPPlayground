//
// Created by rick on 2021/10/11.
//

#include "font_language.h"
#include <cctype>
namespace {
#define SCRIPT_TAG(c1, c2, c3, c4)                                           \
  (((uint32_t)(c1)) << 24 | ((uint32_t)(c2)) << 16 | ((uint32_t)(c3)) << 8 | \
   ((uint32_t)(c4)))

size_t nextDelimiterIndex(const char* buffer,
                          size_t bufferLength,
                          size_t startOffset) {
  for (size_t i = startOffset; i < bufferLength; ++i) {
    if (buffer[i] == '-' || buffer[i] == '_') {
      return i;
    }
  }
  return bufferLength;
}

// Returns true if the buffer is valid for language code
bool isValidLanguageCode(const char* buffer, size_t length) {
  if (length != 2 && length != 3)
    return false;
  if (!std::islower(buffer[0]))
    return false;
  if (!std::islower(buffer[1]))
    return false;
  if (length == 3 && !std::islower(buffer[2]))
    return false;
  return true;
}

// Pack the three letter code into 15 bits and stored to 16 bit integer. The
// highest bit is 0. For the region code, the letters must be all digits in
// three letter case, so the number of possible values are 10. For the language
// code, the letters must be all small alphabets, so the number of possible
// values are 26. Thus, 5 bits are sufficient for each case and we can pack the
// three letter language code or region code to 15 bits.
//
// In case of two letter code, use fullbit(0x1f) for the first letter instead.
uint16_t packLanguageOrRegion(const char* c,
                              size_t length,
                              uint8_t twoLetterBase,
                              uint8_t threeLetterBase) {
  if (length == 2) {
    return 0x7c00u |  // 0x1fu << 10
           (uint16_t)(c[0] - twoLetterBase) << 5 |
           (uint16_t)(c[1] - twoLetterBase);
  } else {
    return ((uint16_t)(c[0] - threeLetterBase) << 10) |
           (uint16_t)(c[1] - threeLetterBase) << 5 |
           (uint16_t)(c[2] - threeLetterBase);
  }
}

// Returns true if buffer is valid for script code. The length of buffer must
// be 4.
bool isValidScriptCode(const char* buffer) {
  return std::isupper(buffer[0]) && std::islower(buffer[1]) &&
         std::islower(buffer[2]) && std::islower(buffer[3]);
}

}  // namespace
namespace minikin {
FontLanguage::FontLanguage(const char* buf, size_t length) {
  size_t firstDelimiterPos = nextDelimiterIndex(buf, length, 0);
  if (isValidLanguageCode(buf, firstDelimiterPos)) {
    language_ = packLanguageOrRegion(buf, firstDelimiterPos, 'a', 'a');
  } else {
    // We don't understand anything other than two-letter or three-letter
    // language codes, so we skip parsing the rest of the string.
    return;
  }

  if (firstDelimiterPos == length) {
    hbLanguage_ = hb_language_from_string(getString().c_str(), -1);
    return;  // Language code only.
  }

  size_t nextComponentStartPos = firstDelimiterPos + 1;
  size_t nextDelimiterPos =
      nextDelimiterIndex(buf, length, nextComponentStartPos);
  size_t componentLength = nextDelimiterPos - nextComponentStartPos;
  if (componentLength == 4) {
    // Possibly script code.
    const char* p = buf + nextComponentStartPos;
    if (isValidScriptCode(p)) {
      script_ = SCRIPT_TAG(p[0], p[1], p[2], p[3]);
      sub_script_bits_ = scriptToSubScriptBits(script_);
    }

    if (nextDelimiterPos == length) {
      hbLanguage_ = hb_language_from_string(getString().c_str(),-1);
      emoji_style_ = resolveEmojiStyle(buf,length,script_);
      return; // No region code.
    }

    nextComponentStartPos = nextDelimiterPos + 1;
    nextDelimiterPos = nextDelimiterIndex(buf,length,nextComponentStartPos);
    componentLength = nextDelimiterPos - nextComponentStartPos;
  }

  if (componentLength == 2 || componentLength == 3) {
    // Possibly region code.
  }
}
std::string FontLanguage::getString() const {
  if (isUnsupported()) {
    return "und";
  }

  char buf[16];
}
uint8_t FontLanguage::scriptToSubScriptBits(uint32_t script) {
  uint8_t subScriptBits = 0u;
  switch(script) {
    case SCRIPT_TAG('B','o','p','o'):
      subScriptBits = kBopomofoFlag;
      break;
    case SCRIPT_TAG('H', 'a', 'n', 'g'):
      subScriptBits = kHangulFlag;
      break;
    case SCRIPT_TAG('H', 'a', 'n', 'b'):
      // Bopomofo is almost exclusively used in Taiwan.
      subScriptBits = kHanFlag | kBopomofoFlag;
      break;
    case SCRIPT_TAG('H', 'a', 'n', 'i'):
      subScriptBits = kHanFlag;
      break;
    case SCRIPT_TAG('H', 'a', 'n', 's'):
      subScriptBits = kHanFlag | kSimplifiedChineseFlag;
      break;
    case SCRIPT_TAG('H', 'a', 'n', 't'):
      subScriptBits = kHanFlag | kTraditionalChineseFlag;
      break;
    case SCRIPT_TAG('H', 'i', 'r', 'a'):
      subScriptBits = kHiraganaFlag;
      break;
    case SCRIPT_TAG('H', 'r', 'k', 't'):
      subScriptBits = kKatakanaFlag | kHiraganaFlag;
      break;
    case SCRIPT_TAG('J', 'p', 'a', 'n'):
      subScriptBits = kHanFlag | kKatakanaFlag | kHiraganaFlag;
      break;
    case SCRIPT_TAG('K', 'a', 'n', 'a'):
      subScriptBits = kKatakanaFlag;
      break;
    case SCRIPT_TAG('K', 'o', 'r', 'e'):
      subScriptBits = kHanFlag | kHangulFlag;
      break;
  }
  return subScriptBits;
}
}  // namespace minikin
