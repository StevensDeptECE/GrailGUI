#pragma once

#include <glad/glad.h>

// GLFW needs to be included after glad. Fixing for entire compile.
#include <GLFW/glfw3.h>

#include "opengl/Shader.hh"
#include "opengl/Shape.hh"

// generic function to generate a vertex buffer
template <typename T>
inline void Shape::gen(uint32_t& vb, std::vector<T>& list) {
  glGenBuffers(1, &vb);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(T) * list.size(), &list[0],
               GL_STATIC_DRAW);
}

// generic function to generate a vertex buffer and set up a single vertex attrib pointer
template <typename T>
inline void Shape::gen(uint32_t& vb, std::vector<T>& list, uint32_t stride,   uint32_t shaderParam, uint32_t numElements, uint64_t offset) {
  get(vb, list);
	glVertexAttribPointer(shaderParam, numElements, GL_FLOAT, GL_FALSE, stride, (void*)offset);
}


// generic function to generate a vertex buffer and set up two vertex attrib pointers
template <typename T>
inline void Shape::gen(uint32_t& vb, std::vector<T>& list, uint32_t stride,
                       uint32_t shaderParam1, uint32_t numElements1,  uint64_t offset1,
                       uint32_t shaderParam2, uint32_t numElements2, uint64_t offset2) {
  get(vb, list);
	glVertexAttribPointer(shaderParam1, numElements1, GL_FLOAT, GL_FALSE, stride, (void*)offset1);
	glVertexAttribPointer(shaderParam2, numElements2, GL_FLOAT, GL_FALSE, stride, (void*)offset2);
}
