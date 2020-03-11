#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D brightness01;
uniform sampler2D brightness02;
uniform sampler2D brightness03;
uniform sampler2D brightness04;

void main()
{      

    vec4 temp_color = texture(scene, TexCoords) + texture(brightness01, TexCoords)/4.0 + texture(brightness02, TexCoords)/4.0 
            + texture(brightness03, TexCoords)/4.0 + texture(brightness04, TexCoords)/4.0;       
    FragColor = 1.0f - exp(-temp_color * 0.5) ;
}