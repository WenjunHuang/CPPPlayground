//
// Created by rick on 2021/9/22.
//
#include "../window.h"
#include "hbshaper.h"
int main() {
  auto lib = std::make_shared<FreeTypeLib>();
  HBShaper latinShaper("../assets/fonts/DejaVuSerif.ttf", lib);
  HBText hbt1 = {"ficellé fffffi. VAV.", "fr", HB_SCRIPT_LATIN,
                 HB_DIRECTION_LTR};
  latinShaper.Init();

  Run(
      []() {
        gl::CreateShaderProgram();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
//        glOrtho(0, width, 0, height, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      },
      [&](int width,int height) {
        glOrtho(0, width, 0, height, -1, 1);
        // render
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        auto meshes = latinShaper.DrawText(hbt1, 20, 320);
        gl::Render(meshes);
        //    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      });
}