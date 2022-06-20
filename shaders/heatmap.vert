#version 330 core
layout (location = 0) in vec3 xy;   // the grid points in slot 0
layout (location = 1) in float inval; // the value corresponding to each grid point comes in separately

uniform mat4 transform;

//out vec2 TexCoord;
out float val;
//Add a uniform bool to check if to use vertColor or Solid color
void main() {
	gl_Position = transform*vec4(xy, 1.0);
	//TexCoord = vec2(0.0, inval);
	val = val;
}

