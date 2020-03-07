#version 330

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

layout (location = 0) in vec4 vPosition;

void main(void)
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vPosition;
}