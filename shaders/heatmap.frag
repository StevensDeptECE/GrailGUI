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
	float f = (val - minVal)/ (maxVal-minVal);
	vec3 c = minColor*(1.0-f) + maxColor * f;
  //vec3 c(1.0,0.0,1.0);
  //FragColor = vec4(1,1,0,1);//texture(ourTexture, f);
	FragColor = vec4(c, 1.0);
}
