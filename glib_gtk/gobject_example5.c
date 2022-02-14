//
// Created by rick on 2022/2/11.
//
#include <glib-object.h>

#define T_TYPE_DOUBLE (t_double_get_type())

G_DECLARE_FINAL_TYPE(TDouble, t_double, T, DOUBLE, GObject)

static guint t_double_signal;
struct _TDouble {
    GObject parent;
    double value;
};

G_DEFINE_TYPE(TDouble, t_double, G_TYPE_OBJECT)

static void t_double_class_init(TDoubleClass *class) {
  t_double_signal = g_signal_new("div-by-zero",
                                 G_TYPE_FROM_CLASS(class),
                                 G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                                 0,
                                 NULL,
                                 NULL,
                                 NULL,
                                 G_TYPE_NONE,
                                 0);
}

static void t_double_init(TDouble *d) {}

gboolean t_double_get_value(TDouble *d, double *value) {
  g_return_val_if_fail(T_IS_DOUBLE(d), FALSE);

  *value = d->value;
  return TRUE;
}

void t_double_set_value(TDouble *d, double value) {
  g_return_if_fail(T_IS_DOUBLE(d));
  d->value = value;
}

TDouble *t_double_new(double value) {
  TDouble *d = g_object_new(T_TYPE_DOUBLE, NULL);
  d->value = value;
  return d;
}

TDouble* t_double_div(TDouble *self,TDouble *other) {
  g_return_val_if_fail(T_IS_DOUBLE(self),NULL);
  g_return_val_if_fail(T_IS_DOUBLE(other),NULL);

  double value;

  if((!t_double_get_value(other,&value)))
    return NULL;
  else if (value == 0){
    g_signal_emit(self,t_double_signal,0);
    return NULL;
  }
  return t_double_new(self->value / value);
}

static void div_by_zero_cb(TDouble* d,gpointer user_data){
  g_print("\nError: division by zero.\n\n");
}

int main() {
  TDouble *d1,*d2,*d3;
  double v1,v3;

  d1 = t_double_new(10.0);
  d2 = t_double_new(20.0);

  g_signal_connect(d1,"div-by-zero",G_CALLBACK(div_by_zero_cb),NULL);
  t_double_set_value(d2,0.0);
  t_double_div(d1,d2);

  g_object_unref(d2);
  g_object_unref(d1);

}

