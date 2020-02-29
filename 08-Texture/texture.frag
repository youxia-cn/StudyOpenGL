#version 330

layout (location = 0) out vec4 color;

in vec3 fNormal;
in vec4 fPosition;
in vec4 fColor;
in vec2 fTexCoord;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;
uniform vec4 lightPosition;
uniform vec4 cameraPosition;
uniform vec3 lightColor;
uniform float texCoordScaleFactor;

uniform sampler2D mapKa;
uniform sampler2D mapKd;
uniform sampler2D mapKs;

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
    vec4 ambientLightColor = vec4(0.5f, 0.5f, 0.5f, 0.5f);
    vec4 ambientColor = texture(mapKa, fTexCoord * texCoordScaleFactor) + vec4(Ka, 1.0f);
    vec4 diffuseColor = texture(mapKd, fTexCoord * texCoordScaleFactor) + vec4(Kd, 1.0f);
    vec4 specularColor = texture(mapKs, fTexCoord * texCoordScaleFactor) + vec4(Ks, 1.0f);
    color = min(vec4(1.0f), 
                ambientColor * ambientLightColor
                + diffuse * diffuseColor * vec4(lightColor, 1.0f) 
                + specular * specularColor * vec4(lightColor, 1.0f)
            );
}