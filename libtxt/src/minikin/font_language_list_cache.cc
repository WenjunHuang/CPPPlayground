//
// Created by rick on 2021/10/11.
//

#include "font_language_list_cache.h"
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <unordered_set>
#include "log/log.h"
#include "minikin_internal.h"
namespace minikin {
const uint32_t FontLanguageListCache::kEmptyListId;

// Returns the text length of output.
static size_t toLanguageTag(char* output,
                            size_t outSize,
                            const std::string& locale) {
  output[0] = '\0';
  if (locale.empty())
    return 0;

  size_t outLength = 0;
  UErrorCode uErr = U_ZERO_ERROR;
  outLength = uloc_canonicalize(locale.c_str(), output, outSize, &uErr);
  if (U_FAILURE(uErr)) {
    // unable to build a proper language identifier
    spdlog::debug("uloc_canonicalize(\"{}\") failed: {}", locale.c_str(),
                  u_errorName(uErr));
    output[0] = '\0';
    return 0;
  }

  // Preserve "und" and "und-****" since uloc_addLikelySubtags changes "und" to
  // "en-Latn-US".
  if (strncmp(output, "und", 3) == 0 &&
      (outLength == 3 || (outLength == 8 && output[3] == '_'))) {
    return outLength;
  }

  char likelyChars[ULOC_FULLNAME_CAPACITY];
  uErr = U_ZERO_ERROR;
  uloc_addLikelySubtags(output, likelyChars, ULOC_FULLNAME_CAPACITY, &uErr);
  if (U_FAILURE(uErr)) {
    // unable to build a proper language identifier
    spdlog::debug("uloc_addLikelySubtags(\"{}\") failed: {}", locale.c_str(),
                  u_errorName(uErr));
    output[0] = '\0';
    return 0;
  }

  uErr = U_ZERO_ERROR;
  outLength = uloc_toLanguageTag(likelyChars, output, outSize, false, &uErr);
  if (U_FAILURE(uErr)) {
    // unable to build a proper language identifier
    spdlog::debug("uloc_toLanguageTag(\"{}\") failed: {}", locale.c_str(),
                  u_errorName(uErr));
    output[0] = '\0';
    return 0;
  }
  return outLength;
}

static std::vector<FontLanguage> parseLanguageList(const std::string& input) {
  std::vector<FontLanguage> result;
  size_t currentIdx = 0;
  size_t commaLoc = 0;

  char langTag[ULOC_FULLNAME_CAPACITY];
  std::unordered_set<uint64_t> seen;
  std::string locale(input.size(), 0);

  while ((commaLoc = input.find_first_of(',', currentIdx)) !=
         std::string::npos) {
    locale.assign(input, currentIdx, commaLoc - currentIdx);
    currentIdx = commaLoc + 1;
    auto length = toLanguageTag(langTag, ULOC_FULLNAME_CAPACITY, locale);

    FontLanguage lang(langTag, length);
    uint64_t identifier = lang.getIdentifier();
    if (!lang.isUnsupported() && seen.count(identifier) == 0) {
      result.push_back(lang);
      if (result.size() == FONT_LANGUAGES_LIMIT) {
        break;
      }
      seen.insert(identifier);
    }
  }

  if (result.size() < FONT_LANGUAGES_LIMIT) {
    locale.assign(input, currentIdx, input.size() - currentIdx);
    size_t length = toLanguageTag(langTag, ULOC_FULLNAME_CAPACITY, locale);
    FontLanguage lang(langTag, length);
    uint64_t identifier = lang.getIdentifier();
    if (!lang.isUnsupported() && seen.count(identifier) == 0) {
      result.push_back(lang);
    }
  }
  return result;
}
FontLanguageListCache* minikin::FontLanguageListCache::getInstance() {
  assertMinikinLocked();
  static FontLanguageListCache* instance = nullptr;
  if (instance == nullptr) {
    instance = new FontLanguageListCache();

    instance->language_lists_.emplace_back();
    instance->language_list_lookup_table_.insert(
        std::make_pair("", kEmptyListId));
  }
  return instance;
}
uint32_t FontLanguageListCache::getId(const std::string& languages) {
  auto inst = FontLanguageListCache::getInstance();
  auto it = inst->language_list_lookup_table_.find(languages);
  if (it != inst->language_list_lookup_table_.end()) {
    return it->second;
  }

  // Given language list is not in cache. Insert it and return newly assigned
  // ID.
  const auto nextId = inst->language_lists_.size();
  FontLanguages fontLanguages(parseLanguageList(languages));
  if (fontLanguages.empty()) {
    return kEmptyListId;
  }
  inst->language_lists_.push_back(std::move(fontLanguages));
  inst->language_list_lookup_table_.insert(std::make_pair(languages, nextId));
  return nextId;
}
const FontLanguages& FontLanguageListCache::getById(uint32_t id) {
  auto inst = FontLanguageListCache::getInstance();
  LOG_ALWAYS_FATAL_IF(id >= inst->language_lists_.size(),
                      "Lookup by unknown language list ID.");
  return inst->language_lists_[id];
}
}  // namespace minikin
