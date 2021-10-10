//
// Created by rick on 2021/9/8.
//

#pragma once
#include "../window.h"
#include "Scenes.h"
class GLFWWindow : public Window {
 public:
  GLFWWindow();
 protected:
  void OnDraw(SkCanvas* canvas) override;
  void OnKey(int key,int action) override;
 private:
  std::unique_ptr<Scenes> scenes_;
};
