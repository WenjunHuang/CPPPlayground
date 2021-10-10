//
// Created by rick on 2021/7/29.
//

#pragma once
#include <skia/core/SkCanvas.h>
#include <QtCore>
#include <array>
#include <chrono>
#include "Scene.h"

using namespace std::chrono;

class Scenes;
class HUD {
 public:
  QHash<QString, QString> extras;

  HUD(Scenes* scenes) : _scenes{scenes} {}

  void draw(SkCanvas* canvas, const Scene* scene, int width, int height);

  void tick();

 private:
  std::array<double, 155> _times{0};

  int _timesIdx = 0;
  time_point<system_clock> t0 = system_clock::now();

  Scenes* _scenes;
};
