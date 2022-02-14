//
// Created by rick on 2022/2/11.
//
#include <glib-object.h>

#define T_TYPE_DOUBLE (t_double_get_type())
struct TDouble {
    GObject parent;
    double value;
};
struct TDoubleClass {
    GObjectClass parent_class;
};

void t_double_class_init(TDoubleClass *clazz) {

}

void t_double_init(TDouble *d) {}

GType t_double_get_type() {
  static GType type = 0;
  if (type == 0) {
    GTypeInfo info{.class_size = sizeof(TDoubleClass),
      .base_init = nullptr,
      .base_finalize = nullptr,
      .class_init=(GClassInitFunc) t_double_class_init,
      .class_finalize = nullptr,
      .class_data = nullptr,
      .instance_size = sizeof(TDouble),
      .n_preallocs = 0,
      .instance_init = (GInstanceInitFunc) t_double_init,
      .value_table = nullptr};
    type = g_type_register_static(G_TYPE_OBJECT, "TDouble", &info, static_cast<GTypeFlags>(0));
  }
  return type;
}

int main() {
  GType dtype;
  TDouble *d = nullptr;
  dtype = t_double_get_type();
  if (dtype != 0)
    g_print("Registration was a success. The type is %lx.\n",dtype);
  else {
    g_print("Registration failed.\n");
    return 1;
  }

  d = static_cast<TDouble *>(g_object_new(T_TYPE_DOUBLE, nullptr));
  if (d)
    g_print("Instantiation was a success. The instance address is %p.\n",d);
  else
    g_print("Instantiation failed.\n");

}

