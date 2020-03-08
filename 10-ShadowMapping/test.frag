#version 330 core
layout(location = 0) out vec4 color;

in vec2 fTexCoord;
uniform sampler2D shadowMap;

void main(){
    color = texture(shadowMap, fTexCoord);
}