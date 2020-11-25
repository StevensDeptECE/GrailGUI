#include "opengl/Canvas.hh"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Style.hh"
#include <GL/gl.h>
#include "opengl/util/Camera.hh"

Canvas::~Canvas() {
  cleanup();
}

void Canvas::cleanup() {
  for (auto s : layers) {
    delete s;
  }
  layers.clear();
  delete cam;
  cam = nullptr;
} 

void Canvas::render() {
  // std::cout << projection << std::endl;
  Shader::useShader(style->getShaderIndex())->setMat4("projection", projection);
  glViewport(vpX,w->height-vpH-vpY,vpW,vpH); 

  for(const auto& l : layers){
    l->render();
  }
}

Camera* Canvas::setLookAtProjection(float eyeX, float eyeY, float eyeZ,
                                 float lookAtX, float lookAtY, float lookAtZ,
                                float upX, float upY, float upZ) {
  cam = new Camera(w->width, w->height, glm::vec3(upX, upY, upZ));
  cam->setCamPos(glm::vec3(eyeX, eyeY, eyeZ));
  cam->setLookingAt(glm::vec3(lookAtX, lookAtY, lookAtZ));
  return cam;
}
