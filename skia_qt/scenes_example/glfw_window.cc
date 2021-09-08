//
// Created by rick on 2021/9/8.
//

#include "glfw_window.h"
GLFWWindow::GLFWWindow() : scenes_(std::make_unique<Scenes>()) {}
void GLFWWindow::OnDraw(SkCanvas* canvas) {
  scenes_->draw(canvas, width_, height_, dpi_, xpos_, ypos_);
}
