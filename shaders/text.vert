#version 330 core
layout (location = 0) in vec3 aPos; // <vec2 pos, vec2 tex>
layout (location = 1) in vec2 UV; // <vec2 pos, vec2 tex>

out vec2 TexCoord;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aPos, 1.0);
    TexCoord = UV;
}  
