#include "opengl/MultiShape3D.hh"

using namespace std;

void MultiShape3D::addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4, uint32_t offset) {
  uint32_t startingIndex = getPointIndex() - offset;

  solidIndices.push_back(startingIndex + i1);
  solidIndices.push_back(startingIndex + i2);
  solidIndices.push_back(startingIndex + i3);
  solidIndices.push_back(startingIndex + i2);
  solidIndices.push_back(startingIndex + i3);
  solidIndices.push_back(startingIndex + i4);
}

void MultiShape3D::genFastRectPrism(float x, float y, float z, uint32_t width, uint32_t length,
                                    uint32_t height, uint32_t texIndex, TexCoordVector &texCoords) {
  // Add the 8 needed vertices
  add3DPoint(x, y, z);
  add3DPoint(x + width, y, z);
  add3DPoint(x, y + height, z);
  add3DPoint(x + width, y + height, z);
  add3DPoint(x, y, z - length);
  add3DPoint(x + width, y, z - length);
  add3DPoint(x, y + height, z - length);
  add3DPoint(x + width, y + height, z - length);
  // Add the duplicate vertices to contain different tex coords
  add3DPoint(x, y + height, z);
  add3DPoint(x + width, y + height, z);
  add3DPoint(x, y, z);
  add3DPoint(x + width, y, z);
  add3DPoint(x + width, y, z);
  add3DPoint(x + width, y + height, z);

  // Add texture coordinates
  if (texCoords.size() != fastRectDistinctVertices) {
    cerr << "Rectangle needs " << fastRectDistinctVertices << " texture points" << endl;
    return;
  }
  for (auto texCoord : texCoords) {
    addTextureCoord(texCoord.first, texCoord.second);
    textureIndices.push_back(texIndex);
  }

  // Connect all the faces
  addRect(0, 1, 2, 3, fastRectDistinctVertices);
  addRect(4, 5, 6, 7, fastRectDistinctVertices);
  addRect(4, 0, 6, 2, fastRectDistinctVertices);
  addRect(5, 12, 7, 13, fastRectDistinctVertices);
  addRect(6, 8, 7, 9, fastRectDistinctVertices);
  addRect(4, 10, 5, 11, fastRectDistinctVertices);
}

void MultiShape3D::genFastCube(float x, float y, float z, uint32_t length, uint32_t texIndex, TexCoordVector &texCoords) {
  genRectPrism(x, y, z, length, length, length, texIndex, texCoords);
}

inline void MultiShape3D::addFace(float x1, float y1, float z1,
   float x2, float y2, float z2,
   float x3, float y3, float z3,
   float x4, float y4, float z4) {
  add3DPoint(x1, y1, z1);
  add3DPoint(x2, y2, z2);
  add3DPoint(x3, y3, z3);
  add3DPoint(x4, y4, z4);
}

void MultiShape3D::genRectPrism(float x, float y, float z, uint32_t width, uint32_t length,
                                uint32_t height, uint32_t texIndex, TexCoordVector &texCoords) {
  addFace(x, y, z,   x+width, y, z,   x, y + height, z, x+width, y+height, z); // Face 1 - front
  addFace(x+width, y, z-length,   x+width, y, z,   x, y + height, z-length, x+width, y+height, z);  // Face 2 - right
#if 0
  add3DPoint(x + width, y, z - length);
  add3DPoint(x + width, y, z);
  add3DPoint(x + width, y + height, z - length);
  add3DPoint(x + width, y + height, z);
#endif
  // Face 3 - back
  add3DPoint(x, y, z - length);
  add3DPoint(x + width, y, z - length);
  add3DPoint(x, y + height, z - length);
  add3DPoint(x + width, y + height, z - length);
  // Face 4 - left
  add3DPoint(x, y, z - length);
  add3DPoint(x, y, z);
  add3DPoint(x, y + height, z - length);
  add3DPoint(x, y + height, z);
  // Face 5 - bottom
  add3DPoint(x, y, z - length);
  add3DPoint(x + width, y, z - length);
  add3DPoint(x, y, z);
  add3DPoint(x + width, y, z);
  // Face 6 - top
  add3DPoint(x, y + height, z - length);
  add3DPoint(x + width, y + height, z - length);
  add3DPoint(x, y + height, z);
  add3DPoint(x + width, y + height, z);

  // Add texture coordinates
  if (texCoords.size() != rectDistinctVertices) {
    cerr << "Rectangle needs " << rectDistinctVertices << " texture points" << endl;
    return;
  }
  for (auto texCoord : texCoords) {
    addTextureCoord(texCoord.first, texCoord.second);
    textureIndices.push_back(texIndex);
  }

  // Connect all the faces
  addRect(0, 1, 2, 3, rectDistinctVertices);
  addRect(4, 5, 6, 7, rectDistinctVertices);
  addRect(8, 9, 10, 11, rectDistinctVertices);
  addRect(12, 13, 14, 15, rectDistinctVertices);
  addRect(16, 17, 18, 19, rectDistinctVertices);
  addRect(20, 21, 22, 23, rectDistinctVertices);
}

void MultiShape3D::genCube(float x, float y, float z, uint32_t length, uint32_t texIndex, TexCoordVector &texCoords) {
  genRectPrism(x, y, z, length, length, length, texIndex, texCoords);
}

void MultiShape3D::genOBJModel(const char *filePath, std::vector<uint32_t> &texIndices, float xOffset, float yOffset, float zOffset) {
  // Loader for OBJ files
  bool loadout = Loader.LoadFile(filePath);

  if (loadout) {
    // if (Loader.LoadedMeshes.size() != textureIndices.size()) {
    //     cerr << "Every model should have a texture" << endl;
    //     return;
    // }
    for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
      // Current model to be drawn
      objl::Mesh curMesh = Loader.LoadedMeshes[i];

      // Add all of the vertex and texture coordinates
      int size = curMesh.Vertices.size();
      for (int j = 0; j < curMesh.Vertices.size(); j++)
      {
        objl::Vertex curVertex = curMesh.Vertices[j];
        add3DPoint(curVertex.Position.X + xOffset,
                   curVertex.Position.Y + yOffset,
                   curVertex.Position.Z + zOffset);
        addTextureCoord(curVertex.TextureCoordinate.X, curVertex.TextureCoordinate.Y);
        textureIndices.push_back(texIndices[i]);
      }

      // Add all of the indices from starting point
      uint32_t startingIndex = getPointIndex() - curMesh.Vertices.size();

      for (int k = 0; k < curMesh.Indices.size(); k++)
      {
        solidIndices.push_back(startingIndex + curMesh.Indices[k]);
      }
    }
  }
  else
  {
    cerr << "Failed to Load File. May have failed to find or not an .obj file." << endl;
  }
}

void MultiShape3D::genOBJModel(const char filePath[]) {
  vector<uint32_t> texIndices = {0};
  genOBJModel(filePath, texIndices);
}

// inline uint32_t MultiShape3D::addTransformedVert(std::vector<float>& vert,
// 													double x, double y, double z,
// 													const Transform* t) {
// 	double xt,yt,zt;
// 	t->transform(x,y,z, xt, yt, zt);
// 	addVert(vert, xt, yt, zt);
// 	return vert.size() - 1;
// }

// void MultiShape3D::genSphere(std::vector<float>& vert,
// 														 std::vector<uint32_t>& ind,
// 														 uint32_t latRes, uint32_t lonRes,
// 														 const Transform* t) {
// 	// first compute a broad band around the sphere excluding the poles
// 	final double dphi = PI<double> / latRes; // vertically only +90 to -90 degrees
// 	double phi = -PI<double> / 2 + dphi;

// 	for (int j = 0; j < latRes - 1; ++j, phi += dphi) {
// 		double z = sin(phi);
// 		double r2 = cos(phi);

// 		double theta = 0;
// 		final double dtheta = PI2<double> / lonRes;
// 		for (int i = 0; i < lonRes; ++i, theta += dtheta) {
// 			double x = r2 * cos(theta), y = r2 * sin(theta);
// 			addTransformedVert(vert, x, y, z, t); // transform and add each vertex
// 		}
// 	}
// 	const uint32_t S = addTransformedVert(vert, 0,0,-1, t); // south pole
// 	const uint32_t N = addTransformedVert(vert, 0,0,+1, t); // north pole

// 	const uint32_t numPoints = latRes * lonRes;
// 	// then create quads with all the points created
// 	for (uint32_t lat = 0, i = 0; lat < latRes; lat++) {
// 		for (int lon = 1; lon < lonRes; lon++, i++) {
// 			addSquare(ind, i, i+1, i+1 + lonRes, i + lonRes);
// 		}
// 		// when we go around the sphere along a line of latitude
// 		// connect back to the first one
// 		addSquare(ind, i, i+1-lonRes, i+1, i+lonRes);
// 	}
// 	// south pole endcap
// 	for (uint lon = 1; lon < lonRes; lon++)
// 		addTri(ind, i-1, i, S);
// 	addTri(in, lonRes-1,0,S);

// 	// north pole endcap
// 	for (uint lon = 1; lon < lonRes; lon++)
// 		addTri(ind, i-1, i, N);
// 	addTri(in, lonRes-1,0, N);
// }
// void MultiShape3D::genCylinder(vector<float>& vert,
// 															 vector<uint32_t>& ind,
// 															 uint32_t lonRes, Transform* t) {
// 	double theta = 0, dt = PI2<double>/lonRes;
// 	double x = 0, y = 0, z = 0;
// 	double xt, yt, zt;
// 	addTransformedVert(vert, x, y, z);
// 	for (uint32_t i = 0; i < lonRes; ++i, theta += dt) {
// 		x = cos(theta), y = sin(theta);
// 		addTransformedVert(vert, x, y, 0, t);
// 		addTransformedVert(vert, x, y, 1, t);
// 	}
// 	for (uint32_t i = 1; i <= lonRes; ++i) {
// 		addSquare(ind, i, i + 1, i + 1 + lonRes, i + lonRes);
// 	}
// 	addSquare(ind, i, 1, 1+lonRes, i+lonRes);
// 	// south pole
// 	const uint32_t S = addTransformedVert(vert, 0,0,-1);	// south pole, transformed
// 																				 const uint32_t S = addVert(vert, xy,yt,zt);
// 	// South facing endcap
// 	for (uint32_t i = 1; i < lonRes; ++i)
// 		addTri(ind, i-1, i, S);
// 	addTri(lonRes-1, 0, S);

// 	const uint32_t N = addTransformedVert(vert, 0,0,+1, t);	// North pole, transformed
// 	// North facing endcap
// 	for (uint32_t i = 1; i < lonRes; ++i)
// 		addTri(ind, i-1, i, N);
// 	addTri(lonRes-1, 0, N);
// }

// void MultiShape3D::tetrahedron(std::vector<float>& vert, const Transform* t) {
// 	addTransformedVert(vert, -1, -1/sqrt(3), -1/sqrt(6),t);
// 	addTransformedVert(vert, +1, -1/sqrt(3), -1/sqrt(6),t);
// 	addTransformedVert(vert, 0,  2/sqrt(3),  -1/sqrt(6),t);
// 	addTransformedVert(vert, 0,  0,           3/sqrt(6),t);
// 	addTri(ind, 0, 1, 2);
// 	addTri(ind, 0, 1, 3);
// 	addTri(ind, 1, 2, 3);
// 	addTri(ind, 2, 0, 3);
// }

// void MultiShape3D::dodecahedron(std::vector<float>& vert, const Transform* t) {

// }

// void MultiShape3D::icosahedron(std::vector<float>& vert,const Transform* t) {

// }

void MultiShape3D::init() {
  //Create VAO,
  // a container to have all shapes and their attributes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  gen(vbo, vertices); // handle for the buffer object for vertices
  //Define parameter 0 in vertex shader contains 3 floats each (x,y,z)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  gen(cbo, colorIndices); // cbo is the handle to buffer object for the colors 
  // Define parameter 1 in vertex shader is 2 floats (texture coord u,v)
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

  gen(tbo, textureIndices); // tbo is the handle for the texture index buffer (up to 32 textures in an array)
  // Define parameter 2 of vertex shader is the texture index (a single integer)
  glVertexAttribIPointer(2, 1, GL_INT, 0, (void *)0);

 //TODO: are the things below even used?
  gen(sbo, solidIndices); // sbo is the handle for index of solid faces
  gen(lbo, lineIndices);  // lbo is the handle for line buffer object
  gen(pbo, pointIndices); // pdo is the handle for point buffer object

  //Multi Texture vertex shader
  shader = Shader::useShader(GLWin::MULTI_TEXTURE_SHADER);
  // Load all of the textures
  cout << textureFiles.size();
  for (auto tf: textureFiles)
    cout << tf << endl;
  texture = new TextureArray(shader, textureFiles.size());
  for (int i = 0; i < textureFiles.size(); i++) {
    texture->loadImage(textureFiles[i], i);
  }
}

void MultiShape3D::render()
{
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  //Multi Texture vertex shader
  shader = Shader::useShader(GLWin::MULTI_TEXTURE_SHADER);
  // Bind all of the textures
  for (int i = 0; i < textureFiles.size(); i++)
  {
    texture->bindImage(i);
  }

  const glm::mat4 mvp = camera->getViewProjection() * transform->getTransform();

  shader->setMat4("projection", *(&mvp));

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, cbo);
  glBindBuffer(GL_ARRAY_BUFFER, tbo);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  //glLineWidth(style->getLineWidth());

  //Draw Solids
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbo);
  glDrawElements(GL_TRIANGLES, solidIndices.size(), GL_UNSIGNED_INT, 0);

  //Draw Lines
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glDrawElements(GL_LINES, lineIndices.size(), GL_UNSIGNED_INT, 0);

  //Draw Points
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pbo);
  glDrawElements(GL_POINTS, pointIndices.size(), GL_UNSIGNED_INT, 0);

  //Unbind
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

// void MultiShape3D::textureSphere(double r,
// 																 uint32_t resLat, uint32_t resLon,
// 																 int imgTexture) {

// }

// void MultiShape3D::wireframeSphere(double r,
// 																	 uint32_t resLat, uint32_t resLon) {

// }

// void MultiShape3D::pointSphere(double r, uint32_t resLat, uint32_t resLon) {

// }

// void MultiShape3D::textureCylinder(double r, double h,
// 																	 uint32_t resLon, int imgTexture) {

// }

// void MultiShape3D::wireframeCylinder(double r, double h, uint32_t resLon) {

// }

// void MultiShape3D::pointCylinder(double r, uint32_t resLat, uint32_t resLon) {

// }

// void MultiShape3D::textureCube(double r, double h,
// 															 uint32_t resLon, int imgTexture) {
// }

// void MultiShape3D::wireframeCube(double r, double h, uint32_t resLon) {

// }

// void MultiShape3D::pointCube(double r, uint32_t resLat, uint32_t resLon) {

// }

// void MultiShape3D::triangleStrip(const float vert[], uint32_t len) {

// }

MultiShape3D::~MultiShape3D()
{
}
