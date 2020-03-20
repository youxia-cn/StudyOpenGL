#version 330 core

layout (location = 0) out vec4 fColor;
in vec2 fTexCoord;

uniform vec2 c;

int iterations = 0;
int max_iterations = 32;
float threshold_squared = 4.0;
vec2 f_z = fTexCoord;

void main(){
    while(iterations < max_iterations && dot(f_z, f_z) < threshold_squared){
        vec2 z_squared;
        z_squared.x = f_z.x * f_z.x - f_z.y * f_z.y;
        z_squared.y = 2.0 * f_z.x * f_z.y;
        f_z = z_squared + c;
        iterations++;
    }
    float gradation = float(iterations) / float(max_iterations) ;
    fColor = vec4(gradation);
}
