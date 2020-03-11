#version 330

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform vec3 lightColor;

void main(void)
{
    FragColor = vec4(lightColor,1.0f);
    // Check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = dot(lightColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(lightColor.rgb, 1.0);
}