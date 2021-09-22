//
// Created by rick on 2021/9/22.
//

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "../util.h"
namespace gl {
GLFWwindow* window;
GLuint program;
GLuint vertex;
GLuint fragment;
GLint position_loc;
GLint uv_loc;

void DeleteGLBuffer(GLuint* buffer) {
  glDeleteBuffers(1, buffer);
  delete buffer;
}
void DeleteGLTexture(GLuint* texture) {
  glDeleteTextures(1, texture);
  delete texture;
}

using GLTexturePtr = std::unique_ptr<GLuint, FunctionWrapper<DeleteGLTexture>>;
using GLBufferPtr = std::unique_ptr<GLuint, FunctionWrapper<DeleteGLBuffer>>;

struct Vertex {
  float x, y;
  float s, t;
  Vertex() {}
  Vertex(float x, float y, float s, float t) : x(x), y(y), s(s), t(t) {}
};

struct Mesh {
  std::unique_ptr<unsigned short[]> indices;
  unsigned int nb_indices;
  std::unique_ptr<unsigned char[]> texture_data;
  std::unique_ptr<Vertex[]> vertices;
  unsigned int nb_vertices;

  GLTexturePtr texture_id;
  GLBufferPtr vertex_buffer;
  GLBufferPtr index_buffer;
};

static const GLchar* vertex_src =
    "attribute vec4 a_position;\n"
    "attribute vec2 a_uv;\n"
    "varying vec2 f_uv;\n"
    "void main() {\n"
    "gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * a_position;\n"
    "f_uv = a_uv;\n"
    "}\n";

GLTexturePtr get_texture_id(int width, int height) {
  GLuint texture_id;

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_ALPHA,
               GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  return GLTexturePtr(new GLuint(texture_id));
}

void UploadTextureData(GLuint* texture_id,
                       int width,
                       int height,
                       const unsigned char* data) {
  glBindTexture(GL_TEXTURE_2D, *texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_ALPHA,
               GL_UNSIGNED_BYTE, data);
}
}  // namespace gl
