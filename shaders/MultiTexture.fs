#version 420 core
out vec4 FragColor;

in vec2 TexCoord;
flat in int TexIndex;

uniform sampler2D ourTexture[32];


void main()
{
	FragColor = texture(ourTexture[TexIndex], vec2(TexCoord.x, TexCoord.y));
}
