//
// Created by rick on 2021/9/8.
//

#include "glfw_window.h"
GLFWWindow::GLFWWindow() : scenes_(std::make_unique<Scenes>()) {}
void GLFWWindow::OnDraw(SkCanvas* canvas) {
  scenes_->draw(canvas, width_, height_, dpi_, xpos_, ypos_);
}
void GLFWWindow::OnKey(int key,int action) {
  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    scenes_->ShowPrevScene();
  else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    scenes_->ShowNextScene();

}
