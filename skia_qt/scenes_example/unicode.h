//
// Created by rick on 2021/9/18.
//

#pragma once

#include <skia/core/SkSpan.h>
#include <skia/core/SkString.h>
#include <skia/core/SkTypes.h>
#include <unicode/ubidi.h>
#include <unicode/ubrk.h>
#include <unicode/uscript.h>
#include <unicode/ustring.h>
#include <unicode/utext.h>
#include <unicode/utf16.h>
#include <unicode/utf8.h>
#include <unicode/utypes.h>
#include <vector>

class BidiIterator {
 public:
  typedef int32_t Position;
  typedef uint8_t Level;
  struct Region {
    Region(Position start, Position end, Level level)
        : start(start), end(end), level(level) {}
    Position start;
    Position end;
    Level level;
  };
  enum Direction {
    kLTR,
    kRTL,
  };

  virtual ~BidiIterator() = default;
  virtual Position getLength() = 0;
  virtual Level getLevelAt(Position) = 0;
  static void ReorderVisual(const Level runLevels[],
                            int levelsCount,
                            int32_t logicalFromVisual[]);
};

class BreakIterator {
 public:
  typedef int32_t Position;
  typedef int32_t Status;
  virtual ~BreakIterator() = default;
  virtual Position first() = 0;
  virtual Position current() = 0;
  virtual Position next() = 0;
  virtual Position preceding(Position offset) = 0;
  virtual Position following(Position offset) = 0;
  virtual Status status() = 0;
  virtual bool isDone() = 0;
  virtual bool setText(const char utfText8[], int utf8Units) = 0;
  virtual bool setText(const char16_t utfText16[], int utf16Units) = 0;
};

class ScriptIterator {
 public:
  typedef uint32_t ScriptID;
  virtual ~ScriptIterator() = default;
  virtual bool getScript(SkUnichar u, ScriptID* script) = 0;
};

class Unicode {
 public:
  typedef uint32_t CombiningClass;
  typedef uint32_t GeneralCategory;
  enum class TextDirection {
    kLTR,
    kRTL,
  };
  typedef size_t Position;
  typedef uint8_t BidiLevel;
  struct BidiRegion {
    BidiRegion(Position start, Position end, BidiLevel level)
        : start(start), end(end), level(level) {}
    Position start;
    Position end;
    BidiLevel level;
  };
  enum class LineBreakType {
    kSoftLineBreak = 0,
    kHardLineBreak = 100,
  };

  enum class BreakType {
    kWords,
    kGraphemes,
    kLines,
  };
  struct LineBreakBefore {
    LineBreakBefore(Position pos, LineBreakType breakType)
        : pos(pos), breakType(breakType) {}
    Position pos;
    LineBreakType breakType;
  };

  ~Unicode() = default;
  bool isControl(SkUnichar utf8) {
    // 因为控制字符的utf8编码和其unicode code
    // point都是一样的，所以这里无需转换了
    return u_iscntrl(utf8);
  }
  bool isWhitespace(SkUnichar utf8) {
    // 因为控制字符的utf8编码和其unicode code
    // point都是一样的，所以这里无需转换了
    return u_isWhitespace(utf8);
  }
  bool isSpace(SkUnichar utf8) { return u_isspace(utf8); }

  std::string convertUtf16ToUtf8(const std::u16string& utf16) {
    icu::UnicodeString unicodeString(utf16.data(), utf16.length());
    std::string utf8String;
    unicodeString.toUTF8String(utf8String);
    return utf8String;
  }

  // Methods used in Shaper and Text
  std::unique_ptr<BidiIterator> makeBidiIterator(const char16_t text[],
                                                 int count,
                                                 BidiIterator::Direction);
  std::unique_ptr<BidiIterator> makeBidiIterator(const char text[],
                                                 int count,
                                                 BidiIterator::Direction);
  std::unique_ptr<BreakIterator> makeBreakIterator(const char locale[],
                                                   BreakType breakType);
  std::unique_ptr<BreakIterator> makeBreakIterator(BreakType type);
  std::unique_ptr<ScriptIterator> makeScriptIterator();

  // High level methods (that we actually use somewhere=SkParagraph)
  bool getBidiRegions(const char utf8[],
                      int utf8Units,
                      TextDirection dir,
                      std::vector<BidiRegion>* results);
  bool getLineBreaks(const char utf8[],
                     int utf8Units,
                     std::vector<LineBreakBefore>* results);
  bool getWords(const char utf8[],
                int utf8Units,
                std::vector<Position>* results);
  bool getGraphemes(const char utf8[],
                    int utf8Units,
                    std::vector<Position>* results);

  template <typename Callback>
  void forEachCodepoint(const char* utf8,
                        int32_t utf8Units,
                        Callback&& callback) {
    const char* current = utf8;
    const char* end = utf8 + utf8Units;
    while (current < end) {
      auto before = current - utf8;
      int index = 0;
      SkUnichar unichar;
      U8_NEXT_OR_FFFD(current, index, end - current, unichar);
      current = current + index;

      auto after = current - utf8;
      callback(unichar, before, after);
    }
  }

  template <typename Callback>
  void forEachCodepoint(const char16_t* utf16,
                        int32_t utf16Units,
                        Callback&& callback) {
    const char16_t* current = utf16;
    const char16_t* end = utf16 + utf16Units;
    while (current < end) {
      auto before = current - utf16;
      SkUnichar unichar;
      int index = 0;
      U16_NEXT_OR_FFFD(current, index, end - current, unichar);
      current = current + index;

      auto after = current - utf16;
      callback(unichar, before, after);
    }
  }

  template <typename Callback>
  void forEachBidiRegion(const uint16_t utf16[],
                         int utf16Units,
                         BidiIterator::Direction dir,
                         Callback&& callback) {
    auto iter = makeBidiIterator(utf16, utf16Units, dir);
    const uint16_t* start16 = utf16;
    const uint16_t* end16 = utf16 + utf16Units;
    BidiIterator::Level currentLevel = 0;
    BidiIterator::Position pos16 = 0;

    while (pos16 <= iter->getLength()) {
      auto level = iter->getLevelAt(pos16);
      if (pos16 == 0) {
        currentLevel = level;
      } else if (level != currentLevel) {
        callback(pos16, start16 - utf16, currentLevel);
        currentLevel = level;
      }
      if (start16 == end16)
        break;

      SkUnichar u;
      int index = 0;
      U16_NEXT_OR_FFFD(start16, index, end16 - start16, u);
      start16 = start16 + index;
      pos16 += U16_LENGTH(u);
    }
  }

  template <typename Callback>
  void forEachBreak(const char16_t utf16[],
                    int utf16Units,
                    BreakType type,
                    Callback&& callback) {
    auto iter = makeBreakIterator(type);
    iter->setText(utf16, utf16Units);
    auto pos = iter->first();
    do {
      callback(pos, iter->status());
      pos = iter->next();
    } while (!iter->isDone());
  }

  virtual void reorderVisual(const BidiLevel runLevels[],
                             int levelsCount,
                             int32_t logicalFromVisual[]) = 0;
  static std::unique_ptr<Unicode> Make();
};
