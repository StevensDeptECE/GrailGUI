#include <algorithm>

#include "opengl/Rectangle.hh"

class Button : public Rectangle {
 private:
  bool isPressed = false;
  std::vector<Vector> color;
  int count = 0;
  uint32_t cbo;

 public:
  Button(float x, float y, float width, float height)
      : Rectangle(x, y, width, height, GL_TRIANGLE_STRIP), color() {
    // colors by vertices
    color.push_back(*new Vector(0.5, 0.5, 0.5));
    color.push_back(*new Vector(0.5, 0.5, 0.5));
    color.push_back(*new Vector(0.9, 0.9, 0.9));
    color.push_back(*new Vector(0.9, 0.9, 0.9));
  }

  void init() {
    // Create VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create EBO
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
                 &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Create VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size() * 3,
                 &vertices[0], GL_STATIC_DRAW);
    // Points
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 3, &color[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Unbind vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glEnableVertexAttribArray(0);
  }

  void updateButton() {
    std::reverse(color.begin(), color.end());
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 3, &color[0],
                 GL_STATIC_DRAW);
  }

  void render() {
    if (count == 100) {
      updateButton();
      count = 0;
      //  std::cout<<"flip"<<std::endl;
    }
    count++;

    shader.use();
    applyTransform();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
  }
};
