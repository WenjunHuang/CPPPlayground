//
// Created by rick on 2021/10/11.
//

#pragma once
#include <unordered_map>
#include <vector>
#include "font_language.h"

namespace minikin {
class FontLanguageListCache {
 public:
  // A special ID for the empty language list.
  // This value must be 0 since the empty language list is inserted into
  // mLanguageLists by default.
  const static uint32_t kEmptyListId = 0;

  // Returns language list ID for the given string representation of
  // FontLanguages. Caller should acquire a lock before calling the method.
  static uint32_t getId(const std::string& languages);

  // Caller should acquire a lock before calling the method.
  static const FontLanguages& getById(uint32_t  id);
 private:
  FontLanguageListCache() {}
  ~FontLanguageListCache() {}

  // Caller should acquire a lock before calling the method.
  static FontLanguageListCache* getInstance();
  std::vector<FontLanguages> language_lists_;

  // A map from string representation of the font language list to the ID.
  std::unordered_map<std::string,uint32_t> language_list_lookup_table_;
};
}  // namespace minikin
