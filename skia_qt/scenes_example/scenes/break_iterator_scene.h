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

  void DrawSubString(SkCanvas* canvas, const std::u16string& text, int height);
  void DrawCharacter(SkCanvas* canvas, int height, const std::u16string& text);
};
