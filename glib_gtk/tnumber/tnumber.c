//
// Created by rick on 2022/2/11.
//

#include "tnumber.h"

static guint t_number_signal;

G_DEFINE_ABSTRACT_TYPE(TNumber, t_number, G_TYPE_OBJECT)

static void
div_by_zero_default_cb(TNumber *self) {
  g_print("\nError: division by zero.\n\n");
}

static void
t_number_class_init(TNumberClass *class) {
  // virtual functions
  class->add = NULL;
  class->to_s = NULL;

  // default signals
  class->div_by_zero = div_by_zero_default_cb;

  // signals
  t_number_signal = g_signal_new("div-by-zero",
                                 G_TYPE_FROM_CLASS(class),
                                 G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                                 G_STRUCT_OFFSET(TNumberClass, div_by_zero),
                                 NULL,
                                 NULL,
                                 NULL,
                                 G_TYPE_NONE,
                                 0);
}

static void
t_number_init(TNumber *inst){}

TNumber* t_number_add(TNumber *self,TNumber *other){
  g_return_val_if_fail(T_IS_NUMBER(self),NULL);
  g_return_val_if_fail(T_IS_NUMBER(other),NULL);

  TNumberClass *class = T_NUMBER_GET_CLASS(self);
  return class->add ? class->add(self,other):NULL;
}

char *
t_number_to_s(TNumber *self) {
  g_return_val_if_fail(T_IS_NUMBER(self),NULL);

  TNumberClass *class = T_NUMBER_GET_CLASS(self);
}