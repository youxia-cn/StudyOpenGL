#version 330

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

out vec4 fColor;

void main(void)
{
    fColor = vColor;
    gl_Position = projection_matrix * view_matrix * model_matrix * vPosition;
}