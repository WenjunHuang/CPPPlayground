//
// Created by rick on 2021/10/11.
//

#pragma once

#include <harfbuzz/hb.h>
#include <cstdint>
#include <string>
#include <vector>

namespace minikin {
// Due to the limits in font fallback score calculation, we can't use anything
// more than 12 languages.
constexpr size_t FONT_LANGUAGES_LIMIT = 12;

// The language or region code is encoded to 15bits.
constexpr uint16_t INVALID_CODE = 0x7fff;

class FontLanguages;
// FontLanguage is a compact representation of a BCP 47 language tag. It
// does not capture all possible information, only what directly affects
// font rendering.
class FontLanguage {
 public:
  enum EmojiStyle : uint8_t {
    EMSTYLE_EMPTY = 0,
    EMSTYLE_DEFAULT = 1,
    EMSTYLE_EMOJI = 2,
    EMSTYLE_TEXT = 3,
  };
  // Default constructor creates the unsupported language.
  FontLanguage()
      : script_(0ul),
        language_(INVALID_CODE),
        region_(INVALID_CODE),
        hbLanguage_(HB_LANGUAGE_INVALID),
        sub_script_bits_(0ul),
        emoji_style_(EMSTYLE_EMPTY) {}
  // Parse from string
  FontLanguage(const char* buf, size_t length);

  [[nodiscard]] bool isUnsupported() const { return language_ == INVALID_CODE; }
  [[nodiscard]] EmojiStyle getEmojiStyle() const { return emoji_style_; }
  [[nodiscard]] hb_language_t getHbLanguage() const { return hbLanguage_; }
  [[nodiscard]] bool isEqualScript(const FontLanguage& other) const;

  // Returns true if this script supports the given script. For example, ja-Jpan
  // supports Hira, ja-Hira doesn't support Jpan.
  [[nodiscard]] bool supportsHbScript(hb_script_t script) const;

  [[nodiscard]] std::string getString() const;

  // Calculates a matching score. This score represents how well the input
  // languages cover this language. The maximum score in the language list is
  // returned. 0 = no match, 1 = script match, 2 = script and primary language
  // match.
  int calcScoreFor(const FontLanguages& supported) const;

  uint64_t getIdentifier() const {
    return ((uint64_t)language_ << 49) | ((uint64_t)script_ << 17) |
           ((uint64_t)region_ << 2) | emoji_style_;
  }

 private:
  friend class FontLanguages;  // for FontLanguages constructor

  // ISO 15924 compliant script code. The 4 chars script code are packed into a
  // 32 bit integer.
  uint32_t script_;

  // ISO 639-1 or ISO 639-2 compliant language code.
  // The two- or three-letter language code is packed into a 15 bit integer.
  // mLanguage = 0 means the FontLanguage is unsupported.
  uint16_t language_;

  // ISO 3166-1 or UN M.49 compliant region code. The two-letter or three-digit
  // region code is packed into a 15 bit integer.
  uint16_t region_;

  // The language to be passed HarfBuzz shaper.
  hb_language_t hbLanguage_;

  // For faster comparing, use 7 bits for specific scripts.
  static const uint8_t kBopomofoFlag = 1u;
  static const uint8_t kHanFlag = 1u << 1;
  static const uint8_t kHangulFlag = 1u << 2;
  static const uint8_t kHiraganaFlag = 1u << 3;
  static const uint8_t kKatakanaFlag = 1u << 4;
  static const uint8_t kSimplifiedChineseFlag = 1u << 5;
  static const uint8_t kTraditionalChineseFlag = 1u << 6;

  uint8_t sub_script_bits_;
  EmojiStyle emoji_style_;

  static uint8_t scriptToSubScriptBits(uint32_t script);

  static EmojiStyle resolveEmojiStyle(const char* buf,
                                      size_t length,
                                      uint32_t script);
  // Returns true if the provide subscript bits has the requested subscript
  // bits. Note that this function returns false if the requested subscript bits
  // are empty.
  static bool supportsScript(uint8_t providedBits, uint8_t requestedBits);
};

// An immutable list of languages.
class FontLanguages {
 public:
  explicit FontLanguages(std::vector<FontLanguage>&& languages);
  FontLanguages()
      : union_of_subscript_bits_(0), is_all_the_same_language_(false) {}
  FontLanguages(FontLanguages&&) = default;

  FontLanguages(FontLanguage&) = delete;
  FontLanguages& operator=(FontLanguages&) = delete;

  size_t size() const { return languages_.size(); }
  bool empty() const { return languages_.empty(); }
  const FontLanguage& operator[](size_t n) const { return languages_[n]; }

 private:
  friend struct FontLanguage;  // for calcScoreFor
  std::vector<FontLanguage> languages_;
  uint8_t union_of_subscript_bits_;
  bool is_all_the_same_language_;

  uint8_t getUnionOfSubScriptBits() const { return union_of_subscript_bits_; }
  bool isAllTheSameLanguage() const { return is_all_the_same_language_; }
};
}  // namespace minikin
