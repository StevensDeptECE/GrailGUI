#version 330 core
out vec4 FragColor;

//in vec2 TexCoord;
in float val;
//uniform sampler2D ourTexture;
uniform vec3 minColor;
uniform vec3 maxColor;
uniform float minVal;
uniform float maxVal;
float f = (val - minVal)/ (maxVal-minVal);

void main()
{
	FragColor = //texture(ourTexture, f);
	FragColor = vec4(minColor*(1.0-f) + maxColor * f,1.0);
}
