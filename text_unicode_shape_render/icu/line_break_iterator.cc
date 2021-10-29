//
// Created by rick on 2021/10/25.
//
#include <gtest/gtest.h>
#include <unicode/brkiter.h>
#define UTF16(codepoint) U16_LEAD(codepoint), U16_TRAIL(codepoint)

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
TEST(LineBreak, Property) {
  ULineBreak lbp =
      (ULineBreak)u_getIntPropertyValue(0x41, UCHAR_LINE_BREAK);  // U+0041
  EXPECT_EQ(U_LB_ALPHABETIC, lbp);

  lbp = (ULineBreak)u_getIntPropertyValue(0x2793, UCHAR_LINE_BREAK);
  EXPECT_EQ(U_LB_AMBIGUOUS, lbp);
}
TEST(LineBreakIterator, basic) {
  UErrorCode status = U_ZERO_ERROR;
  auto iterator = icu::BreakIterator::createLineInstance(icu::Locale(), status);
  ASSERT_TRUE(iterator != nullptr);

  UText text = UTEXT_INITIALIZER;
  char16_t utf16[] = u"Hello World";
  utext_openUChars(&text, reinterpret_cast<const UChar*>(utf16),
                   sizeof(utf16) / sizeof(utf16[0]), &status);
  iterator->setText(&text, status);

  EXPECT_EQ(0, iterator->first());
  EXPECT_EQ(6, iterator->next());
  EXPECT_EQ(icu::BreakIterator::DONE, iterator->next());
}

TEST(LineBreakIterator, softHyphen) {
  UErrorCode status = U_ZERO_ERROR;
  auto iterator = icu::BreakIterator::createLineInstance(icu::Locale(), status);

  UText text = UTEXT_INITIALIZER;
  uint16_t buf[] = {'h', 'e', 'l', 0x00AD, 'l', 'o',
                    ' ', 'w', 'o', 'r',    'l', 'd'};
  utext_openUChars(&text, reinterpret_cast<const UChar*>(buf),
                   sizeof(buf) / sizeof(buf[0]), &status);
  iterator->setText(&text, status);

  EXPECT_EQ(0, iterator->first());
  EXPECT_EQ(4, iterator->next());  // icu breaks on soft hyphen
}

TEST(LineBreakIterator, postfixAndPrefix) {
  UErrorCode status = U_ZERO_ERROR;

  UText text = UTEXT_INITIALIZER;
  uint16_t buf[] = {'U', 'S', 0x00A2, ' ', 'J', 'P', 0x00A5};  // US¢ JP¥
  utext_openUChars(&text, reinterpret_cast<const UChar*>(buf),
                   sizeof(buf) / sizeof(buf[0]), &status);
  auto iterator = icu::BreakIterator::createLineInstance(icu::Locale(), status);
  iterator->setText(&text, status);

  EXPECT_EQ(0, iterator->current());

  EXPECT_EQ(4, iterator->next());                             // after CENT SIGN
  EXPECT_EQ(sizeof(buf) / sizeof(buf[0]), iterator->next());  // end of string
}

TEST(LineBreakIterator, zwjEmojiSequences) {
  UErrorCode status = U_ZERO_ERROR;

  UText text = UTEXT_INITIALIZER;
  uint16_t buf[] = {
      // man + zwj + heart + zwj + man
      UTF16(0x1F468),
      0x200D,
      0x2764,
      0x200D,
      UTF16(0x1F468),
      // woman + zwj + heart + zwj + kiss mark + zwj + woman
      UTF16(0x1F469),
      0x200D,
      0x2764,
      0x200D,
      UTF16(0x1F48B),
      0x200D,
      UTF16(0x1F469),
      // eye + zwj + left speech bubble
      UTF16(0x1F441),
      0x200D,
      UTF16(0x1F5E8),
      // CAT FACE + zwj + BUST IN SILHOUETTE
      UTF16(0x1F431),
      0x200D,
      UTF16(0x1F464),
  };
  utext_openUChars(&text, reinterpret_cast<const UChar*>(buf),
                   sizeof(buf) / sizeof(buf[0]), &status);
  auto iterator = icu::BreakIterator::createLineInstance(icu::Locale(), status);
  iterator->setText(&text, status);

  EXPECT_EQ(0, iterator->current());
  EXPECT_EQ(7, iterator->next()); // after man + zwj + heart + zwj + man
  EXPECT_EQ(17,iterator->next()); // after woman + zwj + heart + zwj + woman
}
