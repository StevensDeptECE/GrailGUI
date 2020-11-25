#include "glad/glad.h"
#include "opengl/Cursor.hh"
#include "opengl/Shader.hh"
#include "opengl/Style.hh"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "X11Util.hh"

std::unordered_map<int, Cursor*> Cursor::cursors;
void Cursor::process_input(Inputs *in, float dt) {
#if 0
  if (!GLWin::use_inputs_library) {
    xPos += in->get_dx(0);
    yPos += in->get_dy(0);
    return;
  }
#endif



  //This trans would be set with the deltas and then it just adds it onto the transform
  //glm::vec3 trans(0,0,0);

  float x = 0, y = 0, z = 0;

  if (GLWin::mouse_entered) {
    x = GLWin::m_xpos - xPos;
    y = GLWin::m_ypos - yPos;
    xPos = GLWin::m_xpos;
    yPos = GLWin::m_ypos;
    // pop the buffers
    in->get_dx(id);
    in->get_dy(id);
    GLWin::mouse_entered = 0;
  } else if (GLWin::mouse_leaved) {
    // Not sure if needed anymore
    return;
  } else {
    dt += 1;
    const int v_x = in->get_dx(id);
    const int v_y = in->get_dy(id);

    if (!(v_x || v_y))
      return;

    const double a_x = 1.0 * v_x;
    const double j_x = 0.08 * a_x;
    const double a_y = 1.0 * v_y;
    const double j_y = 0.08 * a_y;

    //s = s + vt + 0.5*a*t^2 + 1/3.0*j*t^3
    x = abs(v_x) > 1 ? v_x*dt + 0.5*a_x*dt*dt + 1.0/3*j_x*dt*dt*dt : 0;
    y = abs(v_y) > 1 ? v_y*dt + 0.5*a_y*dt*dt + 1.0/3*j_y*dt*dt*dt : 0;

    xPos += x;
    yPos += y;

    if (xPos < 0) {
      x -= xPos;
      xPos = 0;
			//TODO:      GLWin::enableMouse();
      X11Util::setCursor(GLWin::w_xpos, GLWin::w_ypos, xPos-1, yPos);
    }
    if (xPos + width > GLWin::Width) {
      x -= xPos + width - GLWin::Width;
      xPos = GLWin::Width - width;
      GLWin::enableMouse();
      X11Util::setCursor(GLWin::w_xpos, GLWin::w_ypos, xPos+width+1, yPos);
    }
    if (yPos < 0) {
      y -= yPos;
      yPos = 0;
      GLWin::enableMouse();
      X11Util::setCursor(GLWin::w_xpos, GLWin::w_ypos, xPos, yPos-1);
    }
    if (yPos + height > GLWin::Height) {
      y -= yPos + height - GLWin::Height;
      yPos = GLWin::Height - height;
      GLWin::enableMouse();
      X11Util::setCursor(GLWin::w_xpos, GLWin::w_ypos, xPos, yPos+height+1);
    }
  }

  glm::vec3 trans(x, y, z);
  transform = glm::translate(transform,trans);
  GLWin::dirty = 1;
}

void Cursor::update() {

}

void Cursor::render() {
  if (!GLWin::use_inputs_library || GLWin::mouse_leaved || !GLWin::window_focused)
    return;

  //Get Shader based on style
  Shader *s = Shader::useShader(Grail::CURSOR_SHADER);

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  s->setMat4("projection",*(parentCanvas->getProjection()));
  s->setMat4("trans",transform);
  s->setVec4("solidColor",style->getBgColor());

  glLineWidth(style->getLineWidth());

  //Draw Solids
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sbo);
  glDrawElements(GL_TRIANGLES,solidIndices.size(),GL_UNSIGNED_INT,0);

  s->setVec4("solidColor",style->getFgColor());
  //Draw Lines
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
  glDrawElements(GL_LINE_LOOP,lineIndices.size(),GL_UNSIGNED_INT,0);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}
