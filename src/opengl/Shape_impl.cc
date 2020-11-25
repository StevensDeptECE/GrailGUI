#pragma once

#include "opengl/Shape.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"

// generic function to generate a vertex buffer
template<typename T>
inline void Shape::gen(uint32_t& vb, std::vector<T>& list) {
  glGenBuffers(1, &vb);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(T) * list.size(), &list[0], GL_STATIC_DRAW);
}
