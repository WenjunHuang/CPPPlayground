//
// Created by xxzyjy on 29/10/2017.
//

#include "Listener.h"

void Listener::onValue(int value) {
  _value = value;
}

void Listener::onValue(QString sender) {
  _sender = sender;
}
