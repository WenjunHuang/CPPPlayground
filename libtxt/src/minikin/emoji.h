//
// Created by rick on 2021/10/20.
//

#pragma once

#include <unicode/uchar.h>
#include <cstdint>
namespace minikin {
// Returns true if c is emoji.
 bool isEmoji(uint32_t c);

// Returns true if c is emoji modifier base.
bool isEmojiBase(uint32_t c);

// Returns true if c is emoji modifier.
bool isEmojiModifier(uint32_t c);

// Bidi override for ICU that knows about new emoji.
UCharDirection emojiBidiOverride(const void* context, UChar32 c);
}  // namespace minikin