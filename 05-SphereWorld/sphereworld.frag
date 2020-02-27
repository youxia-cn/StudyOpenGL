#version 330

layout (location = 0) out vec4 color;

in vec3 fNormal;
uniform vec3 Ka;

void main(void)
{
    color = vec4(Ka, 1.0f);
}