//
// Created by rick on 2022/2/12.
//

#pragma once
#include <glib-object.h>
#include "tnumber.h"

#define T_TYPE_INT (t_int_get_type())
G_DECLARE_FINAL_TYPE(TInt,t_int,T,INT,TNumber)

TInt *t_int_new_with_value(int value);