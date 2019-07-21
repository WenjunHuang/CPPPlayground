//
// Created by xxzyjy on 2019-01-03.
//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <QDebug>

int main(){
  glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::translate(trans,glm::vec3(1.0f,1.0f,0.0f));
  vec = trans * vec;
  qDebug() << vec.x << vec.y << vec.z;
}

