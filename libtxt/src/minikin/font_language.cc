//
// Created by rick on 2021/10/11.
//

#include "font_language.h"
#include <algorithm>
#include <cctype>
namespace {
#define SCRIPT_TAG(c1, c2, c3, c4)                                           \
  (((uint32_t)(c1)) << 24 | ((uint32_t)(c2)) << 16 | ((uint32_t)(c3)) << 8 | \
   ((uint32_t)(c4)))

// Check if a language code supports emoji according to its subtag
static bool isEmojiSubtag(const char* buf,
                          size_t bufLen,
                          const char* subtag,
                          size_t subtagLen) {
  if (bufLen < subtagLen) {
    return false;
  }
  if (strncmp(buf, subtag, subtagLen) != 0) {
    return false;  // no match between two strings
  }
  return (bufLen == subtagLen || buf[subtagLen] == '\0' ||
          buf[subtagLen] == '-' || buf[subtagLen] == '_');
}
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
      hbLanguage_ = hb_language_from_string(getString().c_str(), -1);
      emoji_style_ = resolveEmojiStyle(buf, length, script_);
      return;  // No region code.
    }

    nextComponentStartPos = nextDelimiterPos + 1;
    nextDelimiterPos = nextDelimiterIndex(buf, length, nextComponentStartPos);
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
  switch (script) {
    case SCRIPT_TAG('B', 'o', 'p', 'o'):
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

bool FontLanguage::isEqualScript(const FontLanguage& other) const {
  return other.script_ == script_;
}

bool FontLanguage::supportsHbScript(hb_script_t script) const {
  static_assert(
      SCRIPT_TAG('J', 'p', 'a', 'n') == HB_TAG('J', 'p', 'a', 'n'),
      "The Minikin script and HarfBuzz hb_script_t have different encodings.");
  if (script == script_)
    return true;
  return supportsScript(sub_script_bits_, scriptToSubScriptBits(script));
}

bool FontLanguage::supportsScript(uint8_t providedBits, uint8_t requestedBits) {
  return requestedBits != 0 && (providedBits & requestedBits) == requestedBits;
}
int FontLanguage::calcScoreFor(const FontLanguages& supported) const {
  bool languageScriptMatch = false;
  bool subtagMatch = false;
  bool scriptMatch = false;

  for (size_t i = 0; i < supported.size(); ++i) {
    if (emoji_style_ != EMSTYLE_EMPTY &&
        emoji_style_ == supported[i].emoji_style_) {
      subtagMatch = true;
      if (language_ == supported[i].language_) {
        return 4;
      }
    }

    if (isEqualScript(supported[i]) ||
        supportsScript(supported[i].sub_script_bits_, sub_script_bits_)) {
      scriptMatch = true;
      if (language_ == supported[i].language_) {
        languageScriptMatch = true;
      }
    }
  }

  if (supportsScript(supported.getUnionOfSubScriptBits(), sub_script_bits_)) {
    scriptMatch = true;
    if (language_ == supported[0].language_ &&
        supported.isAllTheSameLanguage()) {
      return 3;
    }
  }

  if (languageScriptMatch) {
    return 3;
  } else if (subtagMatch) {
    return 2;
  } else if (scriptMatch) {
    return 1;
  }
  return 0;
}
FontLanguage::EmojiStyle FontLanguage::resolveEmojiStyle(const char* buf,
                                                         size_t length,
                                                         uint32_t script) {
  // First, lookup emoji subtag.
  // 10 is the length of "-u-em-text", which is the shortest emoji subtag,
  // unnecessary comparison can be avoided if total length is smaller than 10.
  const size_t kMinSubtagLength = 10;
  static const char kPrefix[] = "-u-em-";
  if (length >= kMinSubtagLength) {
    auto pos =
        std::search(buf, buf + length, kPrefix, kPrefix + strlen(kPrefix));
    if (pos != buf + length) {  // found
      pos += strlen(kPrefix);
      auto remainingLength = length - (pos - buf);
      if (isEmojiSubtag(pos, remainingLength, "emoji", 5)) {
        return EMSTYLE_EMOJI;
      } else if (isEmojiSubtag(pos, remainingLength, "text", 4)) {
        return EMSTYLE_TEXT;
      } else if (isEmojiSubtag(pos, remainingLength, "default", 7)) {
        return EMSTYLE_DEFAULT;
      }
    }
  }

  // If no emoji subtag was provided, resolve the emoji style from script code.
  if (script == SCRIPT_TAG('Z', 's', 'y', 'e')) {
    return EMSTYLE_EMOJI;
  } else if (script == SCRIPT_TAG('Z', 's', 'y', 'm')) {
    return EMSTYLE_TEXT;
  }
  return EMSTYLE_EMPTY;
}

FontLanguages::FontLanguages(std::vector<FontLanguage>&& languages)
    : languages_(std::move(languages)) {
  if (languages_.empty())
    return;

  const auto& lang = languages_[0];

  is_all_the_same_language_ = true;
  union_of_subscript_bits_ = lang.sub_script_bits_;
  for (size_t i = 1; i < languages_.size(); ++i) {
    union_of_subscript_bits_ |= languages_[i].sub_script_bits_;
    if (is_all_the_same_language_ &&
        lang.language_ != languages_[i].language_) {
      is_all_the_same_language_ = false;
    }
  }
}
}  // namespace minikin
