//
// Created by rick on 2022/2/12.
//
#include <glib-object.h>
#include "tnumber.h"
#include "tint.h"
#include "tdouble.h"

static void
notify_cb(GObject *gobject, GParamSpec *pspec, gpointer user_data) {
  const char *name = g_param_spec_get_name(pspec);
  if (T_IS_INT(gobject) && strcmp(name, "value") == 0) {
    int i;
    g_object_get(T_INT(gobject), "value", &i, NULL);
    g_print("Property \"%s\" is set to %d.\n", name, i);
  } else if (T_IS_DOUBLE(gobject) && strcmp(name, "value") == 0) {
    double d;
    g_object_get(T_DOUBLE(gobject), "value", &d, NULL);
    g_print("Property \"%s\" is set to %lf.\n", name, d);
  }
}

int main() {
  TNumber *i = T_NUMBER(t_int_new_with_value(0));
  TNumber *d = T_NUMBER(t_double_new_with_value(0.0));

  g_signal_connect(G_OBJECT(i), "notify::value", G_CALLBACK(notify_cb), NULL);
  g_signal_connect(G_OBJECT(d), "notify::value", G_CALLBACK(notify_cb), NULL);

  g_object_set(T_INT(i), "value", 100, NULL);
  g_object_set(T_DOUBLE(d), "value", 12.345, NULL);

  TNumber *num = t_number_add(i, d);
  const char *si = t_number_to_s(i);
  const char *sd = t_number_to_s(d);
  const char *snum = t_number_to_s(num);

  g_print("%s + %s is %s.\n", si, sd, snum);

  g_object_unref(num);
  g_free(snum);

  num = t_number_add(d, i);
  snum = t_number_to_s(num);
  g_print("%s + %s is %s.\n", sd, si, snum);
}