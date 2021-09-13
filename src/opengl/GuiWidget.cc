#include "opengl/GuiWidget.hh"
#include "opengl/Style.hh"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"

void GUIWidget::init() {
  shader = Shader::useShader(GLWin::COMMON_SHADER);
}

void GUIWidget::drawRect(float x, float y, float w, float h, const glm::vec4& color){
	shader->setVec4("solidColor", color);

}

void GUIWidget::drawRect(float x, float y, float w, float h, const glm::vec4& color){
	shader->setVec4("solidColor", color);

}

  void GUIWidget::fillRect(float x, float y, float w, float h, const glm::vec4& color) {

  }

  void GUIWidget::drawCircle(float x, float y, float r, const glm::vec4& color) {

  }

  void GUIWidget::fillCircle(float x, float y, float r, const glm::vec4& color) {

  }

  void GUIWidget::text(float x, float y, const char t[], uint32_t len, const glm::vec4& color) {

  }
