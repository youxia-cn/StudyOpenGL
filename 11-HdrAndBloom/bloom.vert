#version 330 core
uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;
out vec2 TexCoords;

 void main(){
     gl_Position = vPosition;
     TexCoords = vTexCoord;
 }
