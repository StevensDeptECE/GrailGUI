#pragma once

#include <functional>
#include <iostream>

#include "glad/glad.h"
//#include "grail/Grail.hh"
#include "opengl/Style.hh"
#include "obj/loader.h"
#include "opengl/Canvas.hh"
#include "opengl/GLMath.hh"
#include "opengl/GLWin.hh"
#include "opengl/Shader.hh"
#include "opengl/MultiShape.hh"
#include "opengl/util/Camera.hh"
#include "opengl/util/Transformation.hh"
#include "opengl/util/TextureArray.hh"

class MultiShape3D : public MultiShape {
  // Stores x,y pairs for texture coordinates
  typedef const std::vector<std::pair<float, float>> TexCoordVector;

 protected:
  uint32_t elemPerVert;

 private:
  const uint32_t fastRectDistinctVertices = 14;
  const uint32_t rectDistinctVertices = 24;
  Shader* shader = NULL;
  TextureArray* texture = NULL;
  objl::Loader Loader;

  void addFace(float x1, float y1, float z1, float x2, float y2, float z2,
               float x3, float y3, float z3, float x4, float y4, float z4);

 private:
//  Style* style;
  Camera* camera;
  Transformation* transform;
  std::vector<const char*> textureFiles;
  std::vector<uint32_t> textureIndices;
  // Added this in addition to what is inherited for textures
  uint32_t tbo;

  void addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4,
               uint32_t offset);
  void add3DPoint(float x, float y, float z) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
  }
  void addTextureCoord(float x, float y) {
    colorIndices.push_back(x);
    colorIndices.push_back(y);
  }
  uint32_t getPointIndex() { return vertices.size() / elemPerVert; }

 public:
  MultiShape3D(Canvas* canv, Camera* c, const std::vector<const char*>& textureFiles,
               Transformation* t, 
               uint32_t elemPerVert = 3, uint32_t vertCount = 1024,
               uint32_t solidIndCount = 1024, uint32_t lineIndCount = 1024,
               uint32_t pointIndCount = 1024, uint32_t colorIndCount = 1024)
      : MultiShape(canv, vertCount, solidIndCount, lineIndCount, pointIndCount,
                   colorIndCount),
        camera(c),
        transform(t),
        textureFiles(textureFiles),
        elemPerVert(elemPerVert) {
    textureIndices.reserve(textureFiles.size());
  }
  MultiShape3D(Canvas* canv, Camera* c, const char textureFile[],
               Transformation* t,
               uint32_t elemPerVert = 3, uint32_t vertCount = 1024,
               uint32_t solidIndCount = 1024, uint32_t lineIndCount = 1024,
               uint32_t pointIndCount = 1024, uint32_t colorIndCount = 1024)
      : MultiShape3D(canv, c, std::vector<const char*>({textureFile}), t,
       elemPerVert, vertCount,
       solidIndCount, lineIndCount, pointIndCount, colorIndCount) {}

  void init() override;
  void render() override;

  // Experimental, trying to send less than 36 vertices to gpu
  void genFastRectPrism(float x, float y, float z, uint32_t width,
                        uint32_t length, uint32_t height, uint32_t texIndex,
                        TexCoordVector& texCoords);
  void genFastCube(float x, float y, float z, uint32_t length,
                   uint32_t texIndex, TexCoordVector& texCoords);

  void genRectPrism(float x, float y, float z, uint32_t width, uint32_t length,
                    uint32_t height, uint32_t texIndex,
                    TexCoordVector& texCoords);
  void genCube(float x, float y, float z, uint32_t length, uint32_t texIndex,
               TexCoordVector& texCoords);

  void genOBJModel(const char* filePath, std::vector<uint32_t>& texIndices,
                   float xOffset = 0, float yOffset = 0, float zOffset = 0);
  void genOBJModel(const char* filePath);
  MultiShape3D(Canvas* canv, Camera* c, const char textureFile[],
               Transformation* t, const char modelFile[],
               uint32_t elemPerVert = 3, uint32_t vertCount = 1024,
               uint32_t solidIndCount = 1024, uint32_t lineIndCount = 1024,
               uint32_t pointIndCount = 1024, uint32_t colorIndCount = 1024) :
               MultiShape3D(canv, c, textureFile, t, elemPerVert, vertCount,
               solidIndCount, lineIndCount, pointIndCount, colorIndCount) {
                this->genOBJModel(modelFile);
               }

  ~MultiShape3D();

  // static uint32_t addTransformedVert(std::vector<float>& vert,
  // 									double x, double y, double
  // z, 									const Transform* t);

  // static void genSphere(std::vector<float>& vert,
  // 											std::vector<uint32_t>&
  // ind, 											uint32_t latRes, uint32_t lonRes, 											const Transform* t); static void
  // genCylinder(std::vector<float>& vert, 												std::vector<uint32_t>& ind, 												uint32_t
  // lonRes, 												const Transform* t); static void cube(std::vector<float>& vert,
  // const Transform* t); static void tetrahedron(std::vector<float>& vert,
  // const Transform* t); static void dodecahedron(std::vector<float>& vert,
  // const Transform* t); static void icosahedron(std::vector<float>& vert,const
  // Transform* t); void textureSphere(double r, uint32_t resLat, uint32_t
  // resLon, int imgTexture); void wireframeSphere(double r, uint32_t resLat,
  // uint32_t resLon); void pointSphere(double r, uint32_t resLat, uint32_t
  // resLon);

  // void textureCylinder(double r, double h, uint32_t resLon, int imgTexture);
  // void wireframeCylinder(double r, double h, uint32_t resLon);
  // void pointCylinder(double r, uint32_t resLat, uint32_t resLon);

  // void textureCube(double r, double h, uint32_t resLon, int imgTexture);
  // void wireframeCube(double r, double h, uint32_t resLon);
  // void pointCube(double r, uint32_t resLat, uint32_t resLon);

  // void triangleStrip(const float vert[], uint32_t len);
};
