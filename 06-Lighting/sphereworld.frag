#version 330

layout (location = 0) out vec4 color;

in vec3 fNormal;
in vec4 fPosition;
in vec4 fColor;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;
uniform vec4 lightPosition;
uniform vec3 lightColor;

void main(void)
{
    vec3 lightDirection = normalize(vec3(lightPosition - fPosition));
    float diffuse = max(0.0f, dot(lightDirection, fNormal));
    vec3 eyeDirection = vec3(0.0f, 0.0f, 0.0f) - vec3(fPosition);
    vec3 halfVector = normalize(eyeDirection + lightDirection);
    float specular = max(0.0f, dot(halfVector, fNormal));
    if(diffuse == 0.0f){
        specular = 0.0f;
    }else{
        specular = pow(specular, Ns);
    }
    color = vec4(Ka, 1.0f) + vec4(diffuse * Kd * lightColor, 1.0f) + vec4(specular * Ks * lightColor, 1.0f);
}