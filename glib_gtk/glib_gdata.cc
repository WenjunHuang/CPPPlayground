//
// Created by rick on 2022/2/15.
//
#include <glib-object.h>
#include <string>
#include <fmt/format.h>
#include <iostream>

static void destroy_element(void *data) {

}

static void print_str(GQuark id, gpointer data, gpointer user_data) {
  auto str = static_cast<std::string *>(data);
  std::cout << *str << std::endl;
}

int main() {
  GData *data = nullptr;
  g_datalist_init(&data);
  for (int i = 0; i < 100; i++) {
    auto s = new std::string{fmt::format("value_{}", i)};
    g_datalist_set_data_full(&data, g_strdup_printf("data_id_%d", i), static_cast<void *>(s), destroy_element);
  }

  g_datalist_foreach(&data, reinterpret_cast<GDataForeachFunc>(print_str), nullptr);

  g_datalist_clear(&data);
}