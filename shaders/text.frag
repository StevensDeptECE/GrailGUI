#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 textColor;

void main()
{    
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(ourTexture, TexCoord).r);
	FragColor = textColor * sampled;
}  
