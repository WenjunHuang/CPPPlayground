//
// Created by rick on 2021/9/12.
//

#pragma once

#include "../Scene.h"
class BreakIteratorScene : public Scene {
  Q_OBJECT
 public:
  Q_INVOKABLE BreakIteratorScene();
  void draw(SkCanvas* canvas,
            int width,
            int height,
            float scale,
            int xpos,
            int ypos) override;

 private:
  SkFont mono11_;
  int x_ = 0;
  int y_ = 0;
};
