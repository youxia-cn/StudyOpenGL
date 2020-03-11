#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D lastLevel;

void main()
{   vec2 step = 1.0f / textureSize(lastLevel, 0);
    vec4 sample[25];
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            float x_offset = step.x * (j - 2);
            float y_offset = step.y * (i - 2);
            sample[i*5 + j] = texture(lastLevel, TexCoords + vec2(x_offset, y_offset));
        }
    }
    //高斯模糊卷积核
    // 1 4 7 4 1
    // 4 16 26 16 4
    // 7 26 41 26 7
    // 4 16 26 16 4
    // 1 4 7 4 1
    float kernel[25];
    kernel[0] = 1.0f;kernel[1] = 4.0f;kernel[2] = 7.0f;kernel[3] = 4.0f;kernel[4] = 1.0f;
    kernel[5] = 4.0f;kernel[6] = 16.0f;kernel[7] = 26.0f;kernel[8] = 16.0f;kernel[9] = 4.0f;
    kernel[10] = 7.0f;kernel[11] = 26.0f;kernel[12] = 41.0f;kernel[13] = 26.0f;kernel[14] = 7.0f;
    kernel[15] = 4.0f;kernel[16] = 16.0f;kernel[17] = 26.0f;kernel[18] = 16.0f;kernel[19] = 4.0f;
    kernel[20] = 1.0f;kernel[21] = 4.0f;kernel[22] = 7.0f;kernel[23] = 4.0f;kernel[24] = 1.0f;
    
    vec4 temp_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // temp_color = (kernel[0] / 273.0f) * sample[0] + (kernel[1] / 273.0f) * sample[1] + (kernel[2] / 273.0f) * sample[2] + (kernel[3] / 273.0f) * sample[3] + (kernel[4] / 273.0f) * sample[4] 
    //         + (kernel[5] / 273.0f) * sample[5] + (kernel[6] / 273.0f) * sample[6] + (kernel[7] / 273.0f) * sample[7] + (kernel[8] / 273.0f) * sample[8] + (kernel[9] / 273.0f) * sample[9] 
    //         + (kernel[10] / 273.0f) * sample[10] + (kernel[11] / 273.0f) * sample[11] + (kernel[12] / 273.0f) * sample[12] + (kernel[13] / 273.0f) * sample[13] + (kernel[14] / 273.0f) * sample[14]
    //         + (kernel[15] / 273.0f) * sample[15] + (kernel[16] / 273.0f) * sample[16] + (kernel[17] / 273.0f) * sample[17] + (kernel[18] / 273.0f) * sample[18] + (kernel[19] / 273.0f) * sample[19]
    //         + (kernel[20] / 273.0f) * sample[20] + (kernel[21] / 273.0f) * sample[21] + (kernel[22] / 273.0f) * sample[22] + (kernel[23] / 273.0f) * sample[3] + (kernel[24] / 273.0f) * sample[24] ;
    for(int k=0; k<25; k++){
        temp_color += kernel[k] * sample[k];
    }

    FragColor = temp_color / 273.0f;
}