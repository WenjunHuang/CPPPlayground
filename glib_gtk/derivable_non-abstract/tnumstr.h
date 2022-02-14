//
// Created by rick on 2022/2/14.
//

#pragma once

#include <glib-object.h>
#include "tstr.h"
#include "tnumber.h"

#define T_TYPE_NUM_STR (t_num_str_get_type())

G_DECLARE_FINAL_TYPE(TNumStr, t_num_str, T, NUM_STR, TStr)

// type of the numeric string
enum {
    t_none,
    t_int,
    t_double
};

int
t_num_str_is_numeric_string(const char *string);

void t_num_str_set(TNumStr *self, TNumber *num);

TNumber *
t_num_str_get(TNumStr *self);

int t_num_str_get_num_type(TNumStr *self);

TNumStr *t_num_str_new_with_int(int i);

TNumStr *t_num_str_new_with_double(double d);

TNumStr *t_num_str_new_with_tnumber(TNumber *num);

TNumStr *t_num_str_new(void);
