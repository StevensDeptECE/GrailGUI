#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera {
 private:
  glm::mat4 projection;
  glm::mat4 view;

  int screenWidth;
  int screenHeight;
  glm::vec3 camPos;
  glm::vec3 lookingAt;
  glm::vec3 upVector;
  float fov;
  float zNear;
  float zFar;

 public:
  Camera(int screenWidth, int screenHeight,
         const glm::vec3& camPos = glm::vec3(4, 3, 5),
         const glm::vec3& lookingAt = glm::vec3(0, 0, 0),
         const glm::vec3& upVector = glm::vec3(0, 1, 0), float fov = 45.0f,
         float zNear = 0.1f, float zFar = 100.0f)
      : screenWidth(screenWidth),
        screenHeight(screenHeight),
        camPos(camPos),
        lookingAt(lookingAt),
        upVector(upVector),
        fov(fov),
        zNear(zNear),
        zFar(zFar) {}

  void setScreenWidth(int w) { screenWidth = w; }

  void setScreenHeight(int h) { screenHeight = h; }

  void setCamPos(const glm::vec3& pos) { camPos = pos; }
  void translate(float x, float y, float z) {
    setCamPos(camPos + glm::vec3(x, y, z));
  }
  void setLookingAt(const glm::vec3& pos) { lookingAt = pos; }

  void incrCamPos(const glm::vec3& pos) { camPos += pos; }

  void incrLookingAt(const glm::vec3& pos) { lookingAt += pos; }

  void setUpVector(const glm::vec3& pos) { upVector = pos; }

  void setFovRad(float r) { fov = r; }

  void setZnear(float z) { zNear = z; }

  void setZFar(float z) { zFar = z; }

  void zoomIn(float s) {
    glm::vec3 delta = (lookingAt - camPos) * s;
    camPos += delta;
  }

  void zoomIn() {
    zoomIn(0.5);
  }

  void zoomOut() {
    zoomIn(-0.5);
  }

//TODO: make this not inline, move to c++ file, and have multiple different projections
// also, keep more of the computation in the matrices, not computed on the fly each time
//   inline glm::mat4 getViewProjection() {
//     projection = glm::perspective(glm::radians(fov), float(screenWidth) / float(screenHeight), zNear, zFar);
//     view = glm::lookAt(camPos, lookingAt, upVector);
//     return projection * view;
//   }


inline glm::mat4 getViewProjection() {
    projection = glm::perspective(glm::radians(fov), float(screenWidth) / float(screenHeight), zNear, zFar);
    view = glm::lookAt(camPos, lookingAt, upVector);
    //std::cout<< "Screen Width: "<< screenWidth << ", Screen height: " << screenHeight << ", Znear: " << zNear << ", zFar: "<< ", CamPos: " << camPos << " , Looking At: " << lookingAt << ", up Vector: " << upVector << std::endl;
    glm::mat4 temp = projection * view;
   // std::cout << std::string glm::to_string(temp) << std::endl;
   std::cout << "-------------------" << std::endl;
    for (int i=0; i<temp.length(); i++){
      std::cout <<"column " << i << ": " << temp[i].x << ", " << temp[i].y << ", " << temp[i].z << std::endl;
    }
    std::cout << "-------------------" << std::endl;
    return temp;
  }
};
