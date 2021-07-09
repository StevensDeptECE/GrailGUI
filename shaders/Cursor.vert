#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0

uniform mat4 projection;
uniform mat4 trans;
uniform vec4 solidColor;

out vec4 ourColor; // output a color to the fragment shader

//Add a uniform bool to check if to use vertColor or Solid color
void main() {
	gl_Position = projection*trans*vec4(aPos, 1.0);
	ourColor = solidColor; // set ourColor to the input color we got from the vertex data
}

