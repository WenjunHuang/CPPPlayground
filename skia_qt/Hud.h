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

class Scenes;
class HUD {
  public:
    QHash<QString, QString> extras;

    HUD(Scenes* scenes) : _scenes{scenes} {}

    void draw(SkCanvas* canvas, const Scene* scene, int width, int height);

    void tick();

  private:
    std::array<double, 155> _times{};

    int                      _timesIdx = 0;
    time_point<system_clock> t0;

    Scenes* _scenes;
};
