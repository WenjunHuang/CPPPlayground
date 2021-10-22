//
// Created by rick on 2021/10/20.
//

#pragma once
#include <unicode/brkiter.h>
#include <utils/WindowsUtils.h>
#include <memory>
namespace minikin {
class WordBreaker {
 public:
  ~WordBreaker() { finish(); }
  // libtxt extension: always use the default locale so that a cached instance
  // of the ICU break iterator can be reused.
  void setLocale();

  void setText(const uint16_t* data, size_t size);

  // Advance iterator to next word break. Return offset, or -1 if EOT
  ssize_t next();

  // Current offset of iterator, equal to 0 at BOT or last return from next()
  ssize_t current() const;

  // After calling next(), wordStart() and wordEnd() are offsets defining the
  // previous word. If wordEnd <= wordStart, it's not a word for the purpose of
  // hyphenation.
  ssize_t wordStart() const;

  ssize_t wordEnd() const;

  int breakBadness() const;

  void finish();

 private:
  int32_t iteratorNext();
  void detectEmailOrUrl();
  ssize_t findNextBreakInEmailOrUrl();
  std::unique_ptr<icu::BreakIterator> break_iterator_;
  UText utext_ = UTEXT_INITIALIZER;
  const uint16_t* text_ = nullptr;
  size_t text_size_;
  ssize_t last_;
  ssize_t current_;
  bool iterator_was_reset_;

  // state for the email address / url detector
  ssize_t scan_offset_;
  bool in_email_or_url_;
};
}  // namespace minikin
