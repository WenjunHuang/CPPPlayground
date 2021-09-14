//
// Created by rick on 2021/9/14.
//
#include <hb.h>
#include <string>

int main() {
  std::string text = "中华人民共和国❤️";
  hb_buffer_t *buf = hb_buffer_create();
  hb_buffer_add_utf8(buf,text.c_str(),text.size(),0,text.size());
  hb_buffer_destroy(buf);
}