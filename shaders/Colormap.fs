#version 330 core

vec4 colormap(float x) {
    return vec4(0.0, clamp(x, 0.0, 1.0), clamp(-0.5 * x + 1.0, 0.0, 1.0), 1.0);
}

out vec4 FragColor;
in float t;

void main()
{
  FragColor = colormap(t);
}
