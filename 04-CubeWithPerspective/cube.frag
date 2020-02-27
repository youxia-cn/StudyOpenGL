#version 330

in vec4 fColor;

layout (location = 0) out vec4 color;

void main(void)
{
    color = fColor;
}