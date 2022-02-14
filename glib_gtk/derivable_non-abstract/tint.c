//
// Created by rick on 2022/2/12.
//

#include "tint.h"
#include "tdouble.h"

#define PROP_INT 1
static GParamSpec *int_property = NULL;


struct _TInt {
    TNumber parent;
    int value;
};

G_DEFINE_TYPE(TInt, t_int, T_TYPE_NUMBER)

static void t_int_init(TInt *d) {}

static void t_int_set_property(GObject *object, guint property_id,
                               const GValue *value,
                               GParamSpec *pspec) {
  TInt *self = T_INT(object);
  if (property_id == PROP_INT) {
    self->value = g_value_get_int(value);
  } else {
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
  }
}

static void t_int_get_property(GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec) {
  TInt *self = T_INT(object);

  if (property_id == PROP_INT) {
    g_value_set_int(value, self->value);
  } else
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
}

static TNumber *
t_int_add(TNumber *self, TNumber *other) {
  g_return_val_if_fail(T_IS_INT(self), NULL);

  if (T_IS_DOUBLE(other)) {
    double d;
    g_object_get(T_DOUBLE(other), "value", &d, NULL);
    return T_NUMBER(t_double_new_with_value((double) (T_INT(self)->value) + d));
  } else {
    int i;
    g_object_get(T_INT(other), "value", &i, NULL);
    return T_NUMBER(t_int_new_with_value(T_INT(self)->value + i));
  }
}

static TNumber *
t_int_sub(TNumber *self, TNumber *other) {
  g_return_val_if_fail(T_IS_INT(self), NULL);
  if (T_IS_DOUBLE(other)) {
    double d;
    g_object_get(T_DOUBLE(other),"value",&d,NULL);
    return T_NUMBER(t_double_new_with_value((double)(T_INT(self)->value) - d));
  } else {
    int i;
    g_object_get(T_INT(other), "value", &i, NULL);
    return T_NUMBER(t_int_new_with_value(T_INT(self)->value + i));
  }
}

static TNumber *
t_int_uminus(TNumber *self) {
  g_return_val_if_fail(T_IS_INT(self), NULL);

  return T_NUMBER(t_int_new_with_value(-T_INT(self)->value));
}

static char *
t_int_to_s(TNumber *self) {
  g_return_val_if_fail(T_IS_INT(self), NULL);
  int i;
  g_object_get(T_INT(self), "value", &i, NULL);
  return g_strdup_printf("%d", i);
}

static void
t_int_class_init(TIntClass *class) {
  TNumberClass *tNumberClass = T_NUMBER_CLASS(class);
  GObjectClass *gObjectClass = G_OBJECT_CLASS(class);

  // override virtual functions
  tNumberClass->add = t_int_add;
  tNumberClass->sub = t_int_sub;
  tNumberClass->uminus = t_int_uminus;
  tNumberClass->to_s = t_int_to_s;

  gObjectClass->set_property = t_int_set_property;
  gObjectClass->get_property = t_int_get_property;
  int_property = g_param_spec_int("value", "val", "Integer value",
                                  G_MININT,
                                  G_MAXINT,
                                  0,
                                  G_PARAM_READWRITE);
  g_object_class_install_property(gObjectClass, PROP_INT, int_property);
}

TInt *
t_int_new_with_value(int value) {
  TInt *d = g_object_new(T_TYPE_INT, "value", value, NULL);
  return d;
}

TInt *
t_int_new(void) {
  TInt *d = g_object_new(T_TYPE_INT, NULL);
  return d;

}