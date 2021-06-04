#include "opengl/AngledMultiText.hh"

#include "glad/glad.h"
#include "opengl/Canvas.hh"
#include "opengl/Style.hh"

void AngledMultiText::render() {
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  Shader* s = Shader::useShader(GLWin::TEXT_SHADER);
  s->setVec4("textColor", style->getFgColor());
  s->setMat4("projection", *parentCanvas->getProjection() * transform);
  s->setInt("ourTexture", 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);

  // Update data
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  const int windowLen = 128 * 128 * 24;  // preallocate a
  46 * (2 + 3 + 6 + 4) *
      24;  // TODO: This was too small. We need a better policy?
  glBufferSubData(GL_ARRAY_BUFFER, 0, vert.size() * sizeof(float),
                  &vert[0]);  // TODO: Right now we draw the entire string!
  glDrawArrays(GL_TRIANGLES, 0, vert.size());

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
