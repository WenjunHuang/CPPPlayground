//
// Created by rick on 2022/2/11.
//

#pragma once

#include <glib-object.h>

#define T_TYPE_NUMBER (t_number_get_type())

G_DECLARE_DERIVABLE_TYPE(TNumber, t_number, T, NUMBER, GObject)

struct _TNumberClass {
    GObjectClass parent_class;

    TNumber *(*add)(TNumber *self, TNumber *other);

    char *(*to_s)(TNumber *self);

    /* signal */
    void (*div_by_zero)(TNumber *self);
};

TNumber *t_number_add(TNumber *self, TNumber *other);

char *t_number_to_s(TNumber *self);