#version 330

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 fNormal;
in vec4 fPosition;
in vec4 fColor;
in vec2 fTexCoord;

uniform vec4 lightPosition01;
uniform vec3 lightColor01;
uniform vec4 lightPosition02;
uniform vec3 lightColor02;
uniform vec4 lightPosition03;
uniform vec3 lightColor03;
uniform vec4 lightPosition04;
uniform vec3 lightColor04;

uniform sampler2D mapKd;

void main(void)
{
    vec4 cameraPosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec3 cameraDirection = normalize(vec3(cameraPosition-fPosition));
    float Ns = 80.0f;
    vec4 ambientLightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    vec3 lightDirection01 = normalize(vec3(lightPosition01 - fPosition));
    float distance01 = distance(lightPosition01, fPosition);
    float diffuse01 = max(0.0f, dot(lightDirection01, fNormal));
    vec3 halfVector01 = normalize(cameraDirection + lightDirection01);
    float specular01 = max(0.0f, dot(halfVector01, fNormal));
    if(diffuse01 == 0.0f){
        specular01 = 0.0f;
    }else{
        specular01 = pow(specular01, Ns);
    }  
    vec4 ambientColor = texture(mapKd, fTexCoord * 10.0f) * ambientLightColor;
    vec4 diffuseColor01 = texture(mapKd, fTexCoord * 10.0f) * diffuse01 * vec4(lightColor01, 1.0f) / (distance01);
    vec4 specularColor01 = texture(mapKd, fTexCoord * 10.0f) * specular01 * vec4(lightColor01, 1.0f) / (distance01);
    
    vec3 lightDirection02 = normalize(vec3(lightPosition02 - fPosition));
    float distance02 = distance(lightPosition02, fPosition);
    float diffuse02 = max(0.0f, dot(lightDirection02, fNormal));
    vec3 halfVector02 = normalize(cameraDirection + lightDirection02);
    float specular02 = max(0.0f, dot(halfVector02, fNormal));
    if(diffuse02 == 0.0f){
        specular02 = 0.0f;
    }else{
        specular02 = pow(specular02, Ns);
    }  
    vec4 diffuseColor02 = texture(mapKd, fTexCoord * 10.0f) * diffuse02 * vec4(lightColor02, 1.0f) / (distance02);
    vec4 specularColor02 = texture(mapKd, fTexCoord * 10.0f) * specular02 * vec4(lightColor02, 1.0f) / (distance02);
    
    vec3 lightDirection03 = normalize(vec3(lightPosition03 - fPosition));
    float distance03 = distance(lightPosition03, fPosition);
    float diffuse03 = max(0.0f, dot(lightDirection03, fNormal));
    vec3 halfVector03 = normalize(cameraDirection + lightDirection03);
    float specular03 = max(0.0f, dot(halfVector03, fNormal));
    if(diffuse03 == 0.0f){
        specular03 = 0.0f;
    }else{
        specular03 = pow(specular03, Ns);
    }  
    vec4 diffuseColor03 = texture(mapKd, fTexCoord * 10.0f) * diffuse03 * vec4(lightColor03, 1.0f) / (distance03);
    vec4 specularColor03 = texture(mapKd, fTexCoord * 10.0f) * specular03 * vec4(lightColor03, 1.0f) / (distance03);
    
    vec3 lightDirection04 = normalize(vec3(lightPosition04 - fPosition));
    float distance04 = distance(lightPosition04, fPosition);
    float diffuse04 = max(0.0f, dot(lightDirection04, fNormal));
    vec3 halfVector04 = normalize(cameraDirection + lightDirection04);
    float specular04 = max(0.0f, dot(halfVector04, fNormal));
    if(diffuse04 == 0.0f){
        specular04 = 0.0f;
    }else{
        specular04 = pow(specular04, Ns);
    }  
    vec4 diffuseColor04 = texture(mapKd, fTexCoord * 10.0f) * diffuse04 * vec4(lightColor04, 1.0f) / (distance04);
    vec4 specularColor04 = texture(mapKd, fTexCoord * 10.0f) * specular04 * vec4(lightColor04, 1.0f) / (distance04);
    
    vec4 temp_color = ambientColor + diffuseColor01 + specularColor01 + diffuseColor02 + specularColor02 + diffuseColor03 + specularColor03 + diffuseColor04 + specularColor04;
    
    FragColor = temp_color;
    // Check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = dot(temp_color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(temp_color.rgb, 1.0);
}