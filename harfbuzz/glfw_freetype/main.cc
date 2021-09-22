//
// Created by rick on 2021/9/22.
//
#include "hbshaper.h"
#include "../window.h"
int main(){
  Run([](){
    // render
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
//    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  });
}