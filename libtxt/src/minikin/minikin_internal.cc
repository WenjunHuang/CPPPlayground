//
// Created by rick on 2021/10/11.
//
#include "minikin_internal.h"
namespace minikin {
std::recursive_mutex gMinikinLock;
void assertMinikinLocked() {
#ifdef ENABLE_RACE_DETECTION
  LOG_AWAYS_FATAL_IF(gMinikinLock.tryLock() == 0);
#endif
}
}  // namespace minikin
