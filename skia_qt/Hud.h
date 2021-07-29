//
// Created by rick on 2021/7/29.
//

#pragma once
#include "Scene.h"
#include <QtCore>
#include <array>
#include <chrono>
#include <skia/core/SkCanvas.h>

using namespace std::chrono;

class HUD {
public:
  QHash<QString, QString> extras;
  void draw(SkCanvas *canvas, const Scene* scene, int width, int height);
  void tick() {
    auto t1 = system_clock::now();
    _times[_timesIdx] = duration_cast<nanoseconds>(t1 - t0).count() / 1000000.0;
    t0 = t1;
    _timesIdx = (_timesIdx + 1) % _times.size();
  }

private:
  std::array<double, 155> _times;

  int _timesIdx = 0;
  time_point<system_clock> t0;
};
