#version 330

layout (location = 0) out vec4 color;

in vec3 fNormal;
in vec4 fPosition;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;
uniform vec4 lightPosition;
uniform vec4 cameraPosition;
uniform vec3 lightColor;

void main(void)
{
    vec3 lightDirection = normalize(vec3(lightPosition - fPosition));
    float diffuse = max(0.0f, dot(lightDirection, fNormal));
    vec3 cameraDirection = normalize(vec3(cameraPosition-fPosition));
    vec3 halfVector = normalize(cameraDirection + lightDirection);
    float specular = max(0.0f, dot(halfVector, fNormal));
    if(diffuse == 0.0f){
        specular = 0.0f;
    }else{
        specular = pow(specular, Ns);
    }
    color = min(vec4(1.0f), vec4(Ka, 1.0f) + vec4(diffuse * Kd * lightColor, 1.0f) + vec4(specular * Ks * lightColor, 1.0f));
}