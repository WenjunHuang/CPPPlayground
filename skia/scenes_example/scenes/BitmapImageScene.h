//
// Created by HUANG WEN JUN on 2021/9/9.
//

#pragma once

#include <memory>
#include "../Scene.h"

class BitmapImageScenePrivate;
class BitmapImageScene : public Scene {
  Q_OBJECT
 public:
  Q_INVOKABLE BitmapImageScene();
  ~BitmapImageScene() override;

  void draw(SkCanvas* canvas,
            int width,
            int height,
            float scale,
            int xpos,
            int ypos) override;

 private:
  std::unique_ptr<BitmapImageScenePrivate> dptr_;
};
