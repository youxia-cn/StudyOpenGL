#version 330

layout (location = 0) out vec4 color;

in vec3 fNormal;
in vec4 fPosition;
in vec4 fColor;
in vec2 fTexCoord;
in vec4 shadow_coord;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;
uniform vec4 lightPosition;
uniform vec4 cameraPosition;
uniform vec3 lightColor;
uniform float texCoordScaleFactor;
uniform int hasMapKa;
uniform sampler2D mapKa;
uniform int hasMapKd;
uniform sampler2D mapKd;
uniform int hasMapKs;
uniform sampler2D mapKs;
uniform sampler2DShadow depth_texture;

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
    vec4 ambientLightColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    vec4 ambientColor;
    if(hasMapKa!=0){
        ambientColor = texture(mapKa, fTexCoord*texCoordScaleFactor) * vec4(Ka, 1.0f);
    }else{
        ambientColor = vec4(Ka, 1.0f);
    }
    vec4 diffuseColor;
    if(hasMapKd!=0){
        diffuseColor = texture(mapKd, fTexCoord * texCoordScaleFactor) * vec4(Kd, 1.0f);
    }else{
        diffuseColor = vec4(Kd, 1.0f);
    }
    vec4 specularColor;
    if(hasMapKs!=0){
        specularColor = texture(mapKs, fTexCoord * texCoordScaleFactor) * vec4(Ks, 1.0f);
    }else{
        specularColor = vec4(Ks, 1.0f);
    }

    float f = textureProj(depth_texture, shadow_coord);
    //textureProj()函数的功能约等于下面几行代码的功能。
    
    // float closetDepth = (texture(depth_texture, shadow_coord.xy/shadow_coord.w)).r;
    // float currentDepth = shadow_coord.z/shadow_coord.w;
    // float f = 0.0f;
    // if(closetDepth > currentDepth){
    //     f = 1.0f;
    // }
    
    color = min(vec4(1.0f), 
                ambientColor * ambientLightColor
                + f * diffuse * diffuseColor * vec4(lightColor, 1.0f) 
                + f * specular * specularColor * vec4(lightColor, 1.0f)
            );
}