//
// Created by rick on 2021/9/7.
//
#include <memory>
#include "window.h"

static void DeleteCallback(void*, void* context) {
  delete (char*)context;
}

class CustomAllocator : public SkRasterHandleAllocator {
 public:
  bool allocHandle(const SkImageInfo&, Rec* rec) override {
    auto context = new char[4]{'s', 'k', 'i', 'a'};
    rec->fReleaseProc = DeleteCallback;
    rec->fReleaseCtx = context;
    rec->fHandle = context;
    rec->fPixels = context;
    rec->fRowBytes = 4;
    return true;
  }

  void updateHandle(Handle, const SkMatrix&, const SkIRect&) override {}
};
int main() {
  const auto info = SkImageInfo::MakeN32Premul(1, 1);
  auto c2 = SkRasterHandleAllocator::MakeCanvas(
      std::make_unique<CustomAllocator>(), info);
  char* context = (char*)c2->accessTopRasterHandle();
  SkDebugf("context = %.4s\n", context);
}