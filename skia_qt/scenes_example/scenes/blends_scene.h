//
// Created by rick on 2021/9/11.
//

#pragma once

#include "../Scene.h"
class BlendsScene : public Scene {
  Q_OBJECT
 public:
  Q_INVOKABLE BlendsScene();
  void draw(SkCanvas* canvas,
            int width,
            int height,
            float scale,
            int xpos,
            int ypos) override;
};
