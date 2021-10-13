//
// Created by rick on 2021/10/11.
//

#include "font_family.h"
#include "utils/JenkinsHash.h"
#include "minikin_internal.h"
#include "font_language_list_cache.h"

namespace minikin {
FontStyle::FontStyle(int variant, int weight, bool italic) {}

uint32_t FontStyle::pack(int variant, int weight, bool italic) {
  return (weight & kWeightMask) | (italic ? kItalicMask : 0) |
         (variant << kVariantShift);
}
android::hash_t FontStyle::hash() const {
  uint32_t hash = android::JenkinsHashMix(0,bits_);
  hash = android::JenkinsHashMix(hash,language_list_id_);
  return android::JenkinsHashWhiten(hash);
}
uint32_t FontStyle::registerLanguageList(const std::string& languages) {
  std::scoped_lock lock(gMinikinLock);
  return FontLanguageListCache::getId(languages);
}
}  // namespace minikin
