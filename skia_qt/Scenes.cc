//
// Created by rick on 2021/7/29.
//

#include "Scenes.h"
void Scenes::draw(SkCanvas *canvas, int width, int height, float scale,
                  int mouseX, int mouseY) {
  canvas->clear(0xFFFFFFFF);
  if (_current) {
    auto layer = canvas->save();
    _hud->tick();

    auto l2 = canvas->save();
    _hud->draw(canvas, _current.get(), width, height);
    canvas->restoreToCount(l2);
  }
}

Scenes::Scenes() : _hud{std::make_unique<HUD>()} {}
Scenes::~Scenes() {}
