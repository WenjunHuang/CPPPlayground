//
// Created by rick on 2021/10/11.
//

#pragma once
#include <harfbuzz/hb.h>
#include <mutex>
#include "minikin_font.h"

namespace minikin {
// All external Minikin interfaces are designed to be thread-safe.
// Presently, that's implemented by through a global lock, and having
// all external interfaces take that lock.
extern std::recursive_mutex gMinikinLock;

// Aborts if gMinikinLock is not acquired. Do nothing on the release build.
void assertMinikinLocked();


hb_blob_t* getFontTable(const MinikinFont* minikinFont, uint32_t tag);

constexpr uint32_t MAX_UNICODE_CODE_POINT = 0x10FFFF;
// An RAII wrapper for hb_blob_t
class HbBlob {
 public:
  explicit HbBlob(hb_blob_t* blob) : blob_(blob) {}
  ~HbBlob() { hb_blob_destroy(blob_); }

  const uint8_t* get() const {
    auto data = hb_blob_get_data(blob_, nullptr);
    return reinterpret_cast<const uint8_t*>(data);
  }

  size_t size() const { return (size_t)hb_blob_get_length(blob_); }

 private:
  hb_blob_t* blob_;
};

}  // namespace minikin
