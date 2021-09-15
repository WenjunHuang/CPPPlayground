//
// Created by HUANG WEN JUN on 2021/9/15.
//
#include "common.h"

int main() {
  auto rng = views::iota(1) |
             views::for_each([](int x) { return yield(2 * x); }) |
             views::take(4);
  Print(rng);

  auto rng1 = views::iota(1) | views::for_each([](int x) {
                return yield_if(x % 3 == 0 || x % 5 == 0, x);
              }) |
              views::take(4);
  Print(rng1);

  auto rng2 = views::iota(7) | views::for_each([](int x) {
                return yield_from(views::iota(x - 1, x + 1));
              }) |
              views::take(4);
  Print(rng2);

  auto rng3 = views::iota(1) | views::transform([](int x) {
                return yield_from(views::iota(x, x + 2));
              }) |
              views::take(4);
  Print(rng3);
}
