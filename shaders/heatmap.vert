#version 330 core
layout (location = 0) in vec3 xy_v;   // the position variable has attribute position 0
uniform mat4 projection;
uniform mat4 trans;

out vec2 TexCoord;
//Add a uniform bool to check if to use vertColor or Solid color
void main() {
	gl_Position = projection*vec4(xy_v.x, xy_v.y, 0.0, 1.0);
	TexCoord = vec2(0.0, xy_v.z);
}

