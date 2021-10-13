//
// Created by rick on 2021/10/11.
//

#include "font_language_list_cache.h"
#include "minikin_internal.h"
#include "log/log.h"
namespace minikin {
FontLanguageListCache* minikin::FontLanguageListCache::getInstance() {
  assertMinikinLocked();
  static FontLanguageListCache* instance = nullptr;
  if (instance == nullptr) {
    instance = new FontLanguageListCache();

    instance->language_lists_.push_back(FontLanguages());
    instance->language_list_lookup_table_.insert(
        std::make_pair("", kEmptyListId));
  }
  return instance;
}
uint32_t FontLanguageListCache::getId(const std::string& languages) {
  auto inst = FontLanguageListCache::getInstance();
}
}  // namespace minikin
