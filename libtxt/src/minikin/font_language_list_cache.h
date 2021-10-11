//
// Created by rick on 2021/10/11.
//

#pragma once
#include <vector>
#include <unordered_map>

namespace minikin {
class FontLanguageListCache {
 private:
  FontLanguageListCache(){}
  ~FontLanguageListCache(){}

  // Caller should acquire a lock before calling the method.
  static FontLanguageListCache* getInstance();
  std::vector<FontLanguage> language_lists_;

  // A map from string representation of the font language list to the ID.
  std::unordered_map<std::string,uint32_t> language_list_lookup_table);


};
}
