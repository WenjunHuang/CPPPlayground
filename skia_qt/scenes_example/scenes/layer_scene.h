//
// Created by rick on 2021/9/6.
//

#pragma once

#include "../Scene.h"
class LayerScene : public Scene {
  Q_OBJECT
 public:
  void draw(SkCanvas* canvas,
            int width,
            int height,
            float scale,
            int xpos,
            int ypos) override;
};
