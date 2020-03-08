#version 330 core
layout(location = 0) out vec4 color;

void main(){
    color = vec4(gl_FragCoord.zzz, 1.0f);
}