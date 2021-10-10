//
// Created by rick on 2021/9/14.
//
#include <fmt/printf.h>
#include <gtest/gtest.h>
#include <hb.h>
#include <unicode/utf8.h>
#include <cassert>
#include <memory>
#include <string>
#include <iostream>

struct BufferDeleter {
  void operator()(hb_buffer_t* buffer) { hb_buffer_destroy(buffer); }
};

using HBBufferPtr = std::unique_ptr<hb_buffer_t, BufferDeleter>;

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

std::string text = u8"中华人民共和国❤️";

TEST(buffer, AddUtf8) {
  hb_buffer_t* buf = hb_buffer_create();
  hb_buffer_content_type_t content_type = hb_buffer_get_content_type(buf);

  EXPECT_EQ(content_type, HB_BUFFER_CONTENT_TYPE_INVALID);

  hb_buffer_add_utf8(buf, text.c_str(), text.size(), 0, text.size());
  content_type = hb_buffer_get_content_type(buf);
  EXPECT_EQ(content_type, HB_BUFFER_CONTENT_TYPE_UNICODE);

  hb_buffer_destroy(buf);
}

TEST(buffer, SaveSetSegmentProps) {
  hb_buffer_t* buf = hb_buffer_create();
  hb_buffer_add_utf8(buf, text.c_str(), -1, 0, -1);
  hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
  hb_buffer_set_script(buf, HB_SCRIPT_HAN);
  hb_buffer_set_language(buf, hb_language_from_string("zh", -1));

  hb_segment_properties_t saved_props;
  hb_buffer_get_segment_properties(buf, &saved_props);

  hb_buffer_t* buf2 = hb_buffer_create();
  hb_buffer_set_segment_properties(buf2, &saved_props);
  EXPECT_EQ(hb_buffer_get_script(buf2), HB_SCRIPT_HAN);
  auto lang = hb_buffer_get_language(buf2);
  EXPECT_STREQ(hb_language_to_string(lang), "zh");
}

TEST(face, Create) {
  hb_blob_t* blob =
      hb_blob_create_from_file("../assets/fonts/NotoSansSC-Regular.otf");
  EXPECT_NE(blob, hb_blob_get_empty());

  hb_face_t* face = hb_face_create(blob, 0);
  EXPECT_NE(face, hb_face_get_empty());

  hb_font_t* font = hb_font_create(face);

  char text[] = u8"中";
  int i = 0;
  UChar32 code_point;
  U8_NEXT_OR_FFFD(text, i, -1, code_point);

  hb_codepoint_t glyph;
  ASSERT_TRUE(hb_font_get_glyph(font, code_point, 0, &glyph));
  fmt::print("glyphId:{}\n", glyph);

  int32_t x = 0;
  int32_t y = 0;
  hb_font_get_glyph_advance_for_direction(font, glyph, HB_DIRECTION_TTB, &x,
                                          &y);
  fmt::print("horizontal advance: {}, vertical advance: {}\n", x, y);
}

TEST(cluster, Create) {
  hb_unicode_compose_func_t
  hb_blob_t* blob =
      hb_blob_create_from_file("../assets/fonts/NotoColorEmoji.ttf");
  hb_face_t* face = hb_face_create(blob, 0);
  std::cout << hb_face_get_upem(face) << std::endl;

  EXPECT_NE(face, hb_face_get_empty());

  auto buffer = hb_buffer_create();
  hb_buffer_add_utf8(buffer, u8"👨‍👩‍👧‍👦", -1, 0, -1);
  hb_buffer_set_direction(buffer,HB_DIRECTION_LTR);


  auto font = hb_font_create(face);

  hb_shape(font, buffer, nullptr, 0);
  unsigned int glyph_length = 0;
  auto glyph_infos = hb_buffer_get_glyph_infos(buffer,&glyph_length);
  EXPECT_EQ(glyph_length,1);
}

// int main() {
//   std::string text = u8"中华人民共和国❤️";
//   std::u16string utf16_text = u"中华人民共和国❤️";
// }