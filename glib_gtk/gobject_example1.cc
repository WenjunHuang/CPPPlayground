//
// Created by rick on 2022/2/11.
//
#include <glib-object.h>

int main(int argc, char *argv[]) {
  auto instance1 = g_object_new(G_TYPE_OBJECT, nullptr);
  auto instance2 = g_object_new(G_TYPE_OBJECT, nullptr);
  g_print("The address of instance1 is %p\n", instance1);
  g_print("The address of instance2 is %p\n", instance2);

  auto class1 = G_OBJECT_GET_CLASS(instance1);
  auto class2 = G_OBJECT_GET_CLASS(instance2);

  g_object_unref(instance1);
  g_object_unref(instance2);

}

