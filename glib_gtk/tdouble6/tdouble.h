//
// Created by rick on 2022/2/11.
//

#pragma once

#include <glib-object.h>

#define T_TYPE_DOUBLE (t_double_get_type())

G_DECLARE_FINAL_TYPE(TDouble, t_double, T, DOUBLE, GObject)

TDouble *
t_double_add(TDouble *self, TDouble *other);

TDouble *
t_double_sub(TDouble *self, TDouble *other);

TDouble *
t_double_new_with_value(double value);

TDouble *
t_double_new(void);