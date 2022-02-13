//
// Created by rick on 2022/2/13.
//

#include "tdouble.h"
#include "tint.h"

#define PROP_DOUBLE 1
static GParamSpec *double_property = NULL;

struct _TDouble {
    TNumber parent;
    double value;
};

G_DEFINE_TYPE(TDouble, t_double, T_TYPE_NUMBER)

static void
t_double_set_property(GObject *object, guint property_id, const GValue *value,
                      GParamSpec *pspec) {
  TDouble *self = T_DOUBLE(object);
  if (property_id == PROP_DOUBLE) {
    self->value = g_value_get_double(value);
  } else
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
}

static void
t_double_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
  TDouble *self = T_DOUBLE(object);

  if (property_id == PROP_DOUBLE)
    g_value_set_double(value, self->value);
  else
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
}

static void
t_double_init(TDouble *d) {}

static TNumber *
t_double_add(TNumber *self, TNumber *other) {
  g_return_val_if_fail(T_IS_DOUBLE(self), NULL);

  if (T_IS_INT(other)) {
    int i;
    g_object_get(T_INT(other), "value", &i, NULL);
    return T_NUMBER(t_double_new_with_value(T_DOUBLE(self)->value + (double) i));
  } else {
    double d;
    g_object_get(T_DOUBLE(other), "value", &d, NULL);
    return T_NUMBER(t_double_new_with_value(T_DOUBLE(self)->value + d));
  }
}

static TNumber *
t_double_sub(TNumber *self, TNumber *other) {
  g_return_val_if_fail(T_IS_DOUBLE(self), NULL);

  if (T_IS_INT(other)) {
    int i;
    g_object_get(T_INT(other), "value", &i, NULL);
    return T_NUMBER(t_double_new_with_value(T_DOUBLE(self)->value - (double) i));
  } else {
    double d;
    g_object_get(T_DOUBLE(other), "value", &d, NULL);
    return T_NUMBER(t_double_new_with_value(T_DOUBLE(self)->value - d));
  }
}

static TNumber *
t_double_uminus(TNumber *self) {
  g_return_val_if_fail(T_IS_DOUBLE(self), NULL);

  return T_NUMBER(t_double_new_with_value(-T_DOUBLE(self)->value));
}

static char *
t_double_to_s(TNumber *self) {
  g_return_val_if_fail(T_IS_DOUBLE(self), NULL);

  double d;
  g_object_get(T_DOUBLE(self), "value", &d, NULL);
  return g_strdup_printf("%lf", d);
}

static void
t_double_class_init(TDoubleClass *class) {
  TNumberClass *tNumberClass = T_NUMBER_CLASS(class);
  GObjectClass *gObjectClass = G_OBJECT_CLASS(class);

  // virtual functions
  tNumberClass->add = t_double_add;
  tNumberClass->sub = t_double_sub;
  tNumberClass->uminus = t_double_uminus;
  tNumberClass->to_s = t_double_to_s;

  gObjectClass->set_property = t_double_set_property;
  gObjectClass->get_property = t_double_get_property;
  double_property = g_param_spec_double("value", "val", "Double value",
                                        -G_MAXDOUBLE,
                                        G_MAXDOUBLE,
                                        0,
                                        G_PARAM_READWRITE);
  g_object_class_install_property(gObjectClass, PROP_DOUBLE, double_property);
}

TDouble *
t_double_new_with_value(double value) {
  TDouble *d = g_object_new(T_TYPE_DOUBLE, "value", value, NULL);
  return d;
}

TDouble *
t_double_new(void) {
  TDouble *d = g_object_new(T_TYPE_DOUBLE, NULL);
  return d;
}