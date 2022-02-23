#include "CAD/Transformation.hh"
void Transformation::init() {
	//vector of origin points
	glm::vec4 vec(s->x, s->y, s->z, 1.0f);
	vec = transform * vec;
	
	s->x = vec.x; s->y = vec.y; s->z = vec.z;

	s->init();
}

void Transformation::render() {
	// apply the transform
  Shader::useShader(style->getShaderIndex())->setMat4("projection", transform);
	s->render();
}
