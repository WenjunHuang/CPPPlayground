//
// Created by xxzyjy on 29/10/2017.
//

#include "Counter.h"

void Counter::setValue(int value) {
  if (_value != value) {
    _value = value;
    emit valueChanged(value);
    emit changed();
  }
}