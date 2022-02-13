//
// Created by rick on 2022/2/13.
//

#include "tstr.h"

enum {
    PROP_0,
    PROP_STRING,
    N_PROPERTIES
};

static GParamSpec *str_properties[N_PROPERTIES] = {NULL,};

typedef struct {
    char *string;
} TStrPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TStr, t_str, G_TYPE_OBJECT)

static void
t_str_set_property(GObject *object, guint property_id,
                   const GValue *value, GParamSpec *pspec) {
  TStr *self = T_STR(object);
  TStrPrivate *priv = t_str_get_instance_private(self);

  if (property_id == PROP_STRING) {
    if (priv->string)
      g_free(priv->string);
    priv->string = g_strdup(g_value_get_string(value));
  } else
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
}

static void
t_str_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
  TStr *self = T_STR(object);
  TStrPrivate *priv = t_str_get_instance_private(self);

  if (property_id == PROP_STRING)
    g_value_set_string(value, g_strdup(priv->string));
  else
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
}

static void
t_str_finalize(GObject *object) {
  TStr *self = T_STR(object);
  TStrPrivate *priv = t_str_get_instance_private(self);

  if (priv->string)
    g_free(priv->string);
  G_OBJECT_CLASS(t_str_parent_class)->finalize(object);
}

static void
t_str_init(TStr *self) {
  TStrPrivate *priv = t_str_get_instance_private(self);
  priv->string = NULL;
}

static void
t_str_class_init(TStrClass *class) {
  GObjectClass *gObjectClass = G_OBJECT_CLASS(class);

  gObjectClass->finalize = t_str_finalize;
  gObjectClass->set_property = t_str_set_property;
  gObjectClass->get_property = t_str_get_property;
  str_properties[PROP_STRING] = g_param_spec_string("string", "str", "string",
                                                    "",
                                                    G_PARAM_READWRITE);
  g_object_class_install_property(gObjectClass, N_PROPERTIES, str_properties);
}

void t_str_set_string(TStr *self, const char *s) {
  g_return_if_fail(T_IS_STR(self));
  g_object_set(self, "string", s, NULL);
}

char *
t_str_get_string(TStr *self) {
  g_return_val_if_fail(T_IS_STR(self), NULL);

  char *s;
  g_object_get(self, "string", &s, NULL);
  return s;
}

TStr *
t_str_concat(TStr *self, TStr *other) {
  g_return_val_if_fail(T_IS_STR(self), NULL);
  g_return_val_if_fail(T_IS_STR(other), NULL);

  char *s1 = t_str_get_string(self);
  char *s2 = t_str_get_string(other);
  char *s3 = NULL;

  if (s1 && s2)
    s3 = g_strconcat(s1, s2, NULL);
  else if (s1)
    s3 = g_strdup(s1);
  else if (s2)
    s3 = g_strdup(s2);

  TStr *str = t_str_new_with_string(s3);
  if (s1) g_free(s1);
  if (s2) g_free(s2);
  if (s3) g_free(s3);

  return str;
}

TStr*
t_str_new_with_string(const char *s){
  return T_STR(g_object_new(T_TYPE_STR,"string",s,NULL));
}

TStr*
t_str_new(void) {
  return T_STR(g_object_new(T_TYPE_STR,NULL));
}