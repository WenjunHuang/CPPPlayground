//
// Created by rick on 2021/10/20.
//

#include "emoji.h"
namespace minikin {
bool isEmoji(uint32_t c) {
  return u_hasBinaryProperty(c, UCHAR_EMOJI);
}
bool isEmojiBase(uint32_t c) {
  // These two characters were removed from Emoji_Modifier_Base in Emoji 4.0,
  // but we need to keep them as emoji modifier bases since there are fonts and
  // user-generated text out there that treats these as potential emoji bases.
  if (c == 0x1F91D || c == 0x1F93C)
    return true;
  return u_hasBinaryProperty(c, UCHAR_EMOJI_MODIFIER_BASE);
}
bool isEmojiModifier(uint32_t c) {
  return u_hasBinaryProperty(c, UCHAR_EMOJI_MODIFIER);
}
UCharDirection emojiBidiOverride(const void* context, UChar32 c) {
  return u_charDirection(c);
}
}  // namespace minikin
