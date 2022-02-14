//
// Created by rick on 2022/2/11.
//
#include <glib-object.h>
//#include <gobject/gvalue.h>
//#include <gobject/gvaluetypes.h>

void testInit() {
  GValue aValue = G_VALUE_INIT;
  GValue bValue = G_VALUE_INIT;
  guint64 a, b;
  a = 0xdeadbeef;
  g_value_init(&aValue, G_TYPE_UINT64);
  g_value_set_uint64(&aValue, a);

  g_value_init(&bValue,G_TYPE_UINT64);
  g_value_copy(&aValue,&bValue);

  b = g_value_get_uint64(&bValue);
  if (a== b)
    g_print("Yay!! 10 lines of code to copy around a uint64.\n");
  else
    g_print("Are you sure this is not a Z80?\n");
}

int main() {
  testInit();

}
