#version 330

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;
uniform vec4 lightPosition;
uniform vec3 lightColor;

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec4 fColor;
out vec3 fNormal;
out vec2 fTexCoord;
out vec4 fPosition;

void main(void)
{
    mat4 MV_matrix = view_matrix * model_matrix;
    gl_Position = projection_matrix * view_matrix * model_matrix * vPosition;
    fPosition =  MV_matrix * vPosition;
    fNormal = normalize(transpose(inverse(mat3(MV_matrix))) * vNormal);
    fTexCoord = vTexCoord;

}