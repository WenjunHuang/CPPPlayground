//
// Created by rick on 2021/10/11.
//
#include <gtest/gtest.h>
#include <harfbuzz/hb.h>
#include <minikin/hb_font_cache.h>
#include <memory>
#include <mutex>
#include "minikin/minikin_font.h"
#include "minikin/minikin_internal.h"
#include "minikin_font_for_test.h"

namespace minikin {
class HbFontCacheTest : public testing::Test {
 protected:
  void TearDown() override {
    std::scoped_lock guard(gMinikinLock);
    purgeHbFontCacheLocked();
  }
};

TEST_F(HbFontCacheTest, getHbFontLockedTest) {
  auto fontA = std::make_shared<MinikinFontForTest>("./fonts/Regular.ttf");
  auto fontB = std::make_shared<MinikinFontForTest>("./fonts/Bold.ttf");
  auto fontC = std::make_shared<MinikinFontForTest>("./fonts/BoldItalic.ttf");

  std::scoped_lock lock(gMinikinLock);

  // Never return NULL
  EXPECT_NE(nullptr, getHbFontLocked(fontA.get()));
  EXPECT_NE(nullptr, getHbFontLocked(fontB.get()));
  EXPECT_NE(nullptr, getHbFontLocked(fontC.get()));
  EXPECT_NE(nullptr, getHbFontLocked(nullptr));

  // Must return same object if same font object is passed
  EXPECT_EQ(getHbFontLocked(fontA.get()), getHbFontLocked(fontA.get()));
  EXPECT_EQ(getHbFontLocked(fontB.get()), getHbFontLocked(fontB.get()));
  EXPECT_EQ(getHbFontLocked(fontC.get()), getHbFontLocked(fontC.get()));

  // Different object must be returned if the passed minikinFont has different ID
  EXPECT_NE(getHbFontLocked(fontA.get()), getHbFontLocked(fontB.get()));
}

TEST_F(HbFontCacheTest,purgeCacheTest) {
  auto minikinFont = std::make_shared<MinikinFontForTest>("./fonts/Regular.ttf");
  std::scoped_lock lock(gMinikinLock);
  hb_font_t* font = getHbFontLocked(minikinFont.get());
  ASSERT_NE(nullptr,font);

  // Set user data to identify the font object
  hb_user_data_key_t key;
  void* data = (void*)0xdeadbeef;
  hb_font_set_user_data(font,&key,data,nullptr, false);
  ASSERT_EQ(data,hb_font_get_user_data(font,&key));

  purgeHbFontCacheLocked();

  font = getHbFontLocked(minikinFont.get());
  EXPECT_EQ(nullptr,hb_font_get_user_data(font,&key));

}
}  // namespace minikin
