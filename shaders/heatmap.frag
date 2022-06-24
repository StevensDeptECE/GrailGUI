#version 330 core
out vec4 FragColor;

//in vec2 TexCoord;
in float val;
//uniform sampler2D ourTexture;
uniform vec3 minColor;
uniform vec3 maxColor;
uniform float minVal;
uniform float maxVal;

void main()
{
	const float f = (val - minVal)/ (maxVal-minVal);
	FragColor = //texture(ourTexture, f);
	FragColor = minColor*(1-f) + maxColor * f;
}
