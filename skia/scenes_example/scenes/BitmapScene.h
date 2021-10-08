//
// Created by rick on 2021/7/30.
//

#pragma once
#include <skia/core/SkPath.h>
#include <QtCore>
#include <tuple>
#include "../Scene.h"

class BitmapScene : public Scene {
  Q_OBJECT
 public:
  Q_INVOKABLE  BitmapScene();
  void draw(SkCanvas* canvas,
            int width,
            int height,
            float scale,
            int xpos,
            int ypos) override;

 private:
  QRandomGenerator _random{0};
  QVector<std::pair<SkPath, int>> _shapes;
};