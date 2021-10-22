//
// Created by rick on 2021/10/20.
//

#include "word_breaker.h"
#include <mutex>
#include "Hyphenator.h"
#include "emoji.h"
namespace minikin {
const uint32_t CHAR_SOFT_HYPHEN = 0x00AD;
const uint32_t CHAR_ZWJ = 0x200D;

// libtxt extension: avoid the cost of initializing new ICU break iterators
// by constructing a global iterator using the default locale and then
// creating a clone for each WordBreaker instance.
static std::once_flag gLibtxtBreakIteratorInitFlag;
static icu::BreakIterator* gLibtxtDefaultBreakIterator = nullptr;

// Chicago Manual of Style recommends breaking after these characters in URLs
// and email addresses
static bool breakAfter(uint16_t c) {
  return c == ':' || c == '=' || c == '&';
}

// Chicago Manual of Style recommends breaking before these characters in URLs
// and email addresses
static bool breakBefore(uint16_t c) {
  return c == '~' || c == '.' || c == ',' || c == '-' || c == '_' || c == '?' ||
         c == '#' || c == '%' || c == '=' || c == '&';
}

void WordBreaker::setLocale() {
  UErrorCode status = U_ZERO_ERROR;
  std::call_once(gLibtxtBreakIteratorInitFlag, [&status] {
    gLibtxtDefaultBreakIterator =
        icu::BreakIterator::createLineInstance(icu::Locale(), status);
  });
  break_iterator_.reset(gLibtxtDefaultBreakIterator->clone());
  if (text_ != nullptr) {
    break_iterator_->setText(&utext_, status);
  }
  iterator_was_reset_ = true;
}
void WordBreaker::setText(const uint16_t* data, size_t size) {
  text_ = data;
  text_size_ = size;
  iterator_was_reset_ = false;
  last_ = 0;
  current_ = 0;
  scan_offset_ = 0;
  in_email_or_url_ = false;
  UErrorCode status = U_ZERO_ERROR;
  utext_openUChars(&utext_, reinterpret_cast<const UChar*>(data), size,
                   &status);
  break_iterator_->setText(&utext_, status);
  break_iterator_->first();
}
ssize_t WordBreaker::next() {
  last_ = current_;

  detectEmailOrUrl();
  if (in_email_or_url_) {
    current_ = findNextBreakInEmailOrUrl();
  } else {  // Business as usual
    current_ = (ssize_t)iteratorNext();
  }
  return current_;
}

enum ScanState {
  START,
  SAW_AT,
  SAW_COLON,
  SAW_COLON_SLASH,
  SAW_COLON_SLASH_SLASH,
};
void WordBreaker::detectEmailOrUrl() {
  // scan forward from current ICU position for email address or URL
  if (last_ >= scan_offset_) {
    ScanState state = START;
    size_t i;
    for (i = last_; i < text_size_; i++) {
      uint16_t c = text_[i];
      // scan only ASCII characters, stop at space
      if (!(' ' < c && c <= 0x007E)) {
        break;
      }
      if (state == START && c == '@') {
        state = SAW_AT;
      } else if (state == START && c == ':') {
        state = SAW_COLON;
      } else if (state == SAW_COLON || state == SAW_COLON_SLASH) {
        if (c == '/') {
          state = static_cast<ScanState>((int)state +
                                         1);  // next state adds a slash
        } else {
          state = START;
        }
      }
    }
    if (state == SAW_AT || state == SAW_COLON_SLASH_SLASH) {
      if (!break_iterator_->isBoundary(i)) {
        // If there are combining marks or such at the end of the URL or the
        // email address, consider them a part of the URL or the email, and skip
        // to the next actual boundary.
        i = break_iterator_->following(i);
      }
      in_email_or_url_ = true;
      iterator_was_reset_ = true;
    } else {
      in_email_or_url_ = false;
    }
    scan_offset_ = i;
  }
}
ssize_t WordBreaker::findNextBreakInEmailOrUrl() {
  // special rules for email addresses and URL's as per Chicago Manual of Style
  // (16th ed.)
  uint16_t lastChar = text_[last_];
  ssize_t i;
  for (i = last_ + 1; i < scan_offset_; i++) {
    if (breakAfter(lastChar)) {
      break;
    }
    // break after double slash
    if (lastChar == '/' && i >= last_ + 2 && text_[i - 2] == '/') {
      break;
    }
    const uint16_t thisChar = text_[i];
    // never break after hyphen
    if (lastChar != '-') {
      if (breakBefore(thisChar)) {
        break;
      }
      // break before single slash
      if (thisChar == '/' && lastChar != '/' &&
          !(i + 1 < scan_offset_ && text_[i + 1] == '/')) {
        break;
      }
    }
    lastChar = thisChar;
  }
  return i;
}
void WordBreaker::finish() {}

/**
 * Determine whether a line break at position i within the buffer buf is valid.
 *This represents customization beyond the ICU behavior, because plain ICU
 *provides some line break opportunities that we don't want.
 **/
static bool isBreakValid(const uint16_t* buf, size_t bufEnd, size_t i) {
  uint32_t codePoint;
  size_t prevOffset = i;
  U16_PREV(buf, 0, prevOffset, codePoint);

  // Do not break on hard or soft hyphens. These are handled by automatic
  // hyphenation.
  if (Hyphenator::isLineBreakingHyphen(codePoint) ||
      codePoint == CHAR_SOFT_HYPHEN) {
    // txt addition: Temporarily always break on hyphen. Changed from false to
    // true.
    return true;
  }

  // For Myanmar kinzi sequences, created by <consonant, ASAT, VIRAMA,
  // consonant>. This is to go around a bug in ICU line breaking:
  // http://bugs.icu-project.org/trac/ticket/12561. To avoid too much looking
  // around in the strings, we simply avoid breaking after any Myanmar virama,
  // where no line break could be imagined, since the Myanmar virama is a pure
  // stacker.
  if (codePoint == 0x1039) {  // MYANMAR SIGN VIRAMA
    return false;
  }

  uint32_t next_codepoint;
  size_t next_offset = i;
  U16_NEXT(buf, next_offset, bufEnd, next_codepoint);

  // Rule LB8 for Emoji ZWJ sequences. We need to do this ourselves since we may
  // have fresher emoji data than ICU does.
  if (codePoint == CHAR_ZWJ && isEmoji(next_codepoint)) {
    return false;
  }

  // Rule LB30b. We need to this ourselves since we may have fresher emoji data
  // than ICU does.
  if (isEmojiModifier(next_codepoint)) {
    if (codePoint == 0xFE0F && prevOffset > 0) {
      // skip over emoji variation selector
      U16_PREV(buf, 0, prevOffset, codePoint);
    }
    if (isEmojiBase(codePoint)) {
      return false;
    }
  }
  return true;
}
// Customized iteratorNext that takes care of both resets and our modifications
// to ICU's behavior.
int32_t WordBreaker::iteratorNext() {
  int32_t result;
  do {
    if (iterator_was_reset_) {
      result = break_iterator_->following(current_);
      iterator_was_reset_ = false;
    } else {
      result = break_iterator_->next();
    }
  } while (!(result == icu::BreakIterator::DONE ||
             (size_t)result == text_size_ ||
             isBreakValid(text_, text_size_, result)));
  return result;
}
ssize_t WordBreaker::current() const {
  return current_;
}
ssize_t WordBreaker::wordStart() const {
  if (in_email_or_url_) {
    return last_;
  }
  ssize_t result = last_;
  while (result < current_) {
    UChar32 c;
    ssize_t ix = result;
    U16_NEXT(text_, ix, current_, c);
    const int32_t lb = u_getIntPropertyValue(c,UCHAR_LINE_BREAK);
    // strip leading punctuation, defined as OP and QU line breaking classes,
    // see UAX #14
    if (!(lb == U_LB_OPEN_PUNCTUATION || lb == U_LB_QUOTATION)) {
      break;
    }
    result = ix;
  }
  return result;
}
ssize_t WordBreaker::wordEnd() const {
  if (in_email_or_url_)
    return last_;

  ssize_t result = current_;
  while (result > last_) {
    UChar32 c;
    ssize_t ix = result;
    U16_PREV(text_, last_, ix, c);
    const int32_t gc_mask = U_GET_GC_MASK(c);

    // strip trailing space and punctuation
  }
  return 0;
}

}  // namespace minikin
