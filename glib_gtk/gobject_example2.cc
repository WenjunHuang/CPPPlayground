//
// Created by rick on 2022/2/11.
//
#include <glib-object.h>

static void showRefCount(GObject *instance) {
  if (G_IS_OBJECT(instance))
    g_print("Reference count is %d.\n", instance->ref_count);
  else
    g_print("Instance is not GObject.\n");
}

int main(int argc, char *argv[]) {
  auto instance = G_OBJECT(g_object_new(G_TYPE_OBJECT, nullptr));
  g_print("Call g_object_new.\n");
  showRefCount(instance);

  g_object_ref(instance);
  g_print("Call g_object_ref.\n");
  showRefCount(instance);

  g_object_ref(instance);
  g_print("Call g_object_ref.\n");
  showRefCount(instance);

  g_object_ref(instance);
  g_print("Call g_object_ref.\n");
  showRefCount(instance);
}

