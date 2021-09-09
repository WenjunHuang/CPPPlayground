//
// Created by HUANG WEN JUN on 2021/9/9.
//

#include "BitmapImageScene.h"
#include <skia/core/SkImage.h>

struct BitmapImageScenePrivate {
  sk_sp<SkImage> image;
  int x, y;
};

BitmapImageScene::BitmapImageScene() {
  dptr_ = std::make_unique<BitmapImageScenePrivate>();
  dptr_->image =
      SkImage::MakeFromEncoded(resourceAsData("/assets/images/IMG_7098.jpeg"));
  dptr_->x = 0;
  dptr_->y = 0;
}

BitmapImageScene::~BitmapImageScene() = default;

void BitmapImageScene::draw(SkCanvas* canvas,
                            int width,
                            int height,
                            float scale,
                            int xpos,
                            int ypos) {
  canvas->clear(SK_ColorRED);
}
