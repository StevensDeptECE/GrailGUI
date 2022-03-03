#include "CAD/Transformation.hh"
void Transformation::init() {
	//vector of origin points
	
	glm::vec4 vec(s->x, s->y, s->z, 1.0f);
	vec = transform * vec;
	std::cout<<transform<<endl;
	std::cout<<"Hello this should test"<<endl;
}

void Transformation::render() {
	// apply the transform
  Shader::useShader(style->getShaderIndex())->setMat4("projection", transform);
	s->render();
}
