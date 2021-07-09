#version 330 core
layout (location = 0) in vec3 aPos; //position in
layout (location = 1) in vec4 vertColor; //Color per vertex

uniform mat4 projection;
uniform mat4 trans;

out vec4 ourColor;

void main(){
	gl_Position = projection*vec4(aPos,1.0);
	ourColor = vertColor;
}
