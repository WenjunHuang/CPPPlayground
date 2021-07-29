//
// Created by rick on 2021/7/29.
//

#pragma once
#include "Hud.h"
#include <QtCore>
#include <memory>
#include <skia/core/SkCanvas.h>

class Scenes {
public:
  Scenes();
  ~Scenes();
  void draw(SkCanvas *canvas, int width, int height, float scale, int mouseX,
            int mouseY);

private:
  using ScenePtr = std::shared_ptr<Scene>;
  std::shared_ptr<HUD> _hud;
  ScenePtr _current;
  QVector<QString> _scenes;
  QHash<QString, ScenePtr> _scenesMap;
};
