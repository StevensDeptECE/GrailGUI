#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in int aTexIndex;

uniform mat4 projection;

out vec2 TexCoord;
flat out int TexIndex;

void main()
{
	gl_Position = projection*vec4(aPos, 1.0);
	TexCoord = aTexCoord;
	TexIndex = aTexIndex;
}
