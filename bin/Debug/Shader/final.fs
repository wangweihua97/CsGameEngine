#version 430 core
#define PRECISION 0.000001
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D finalTexture;

void main()
{	
    vec4 col = texture(finalTexture, TexCoords).xyzw;
    float a = (1 - step(PRECISION ,col.x)) * (1 - step(PRECISION ,col.y)) * (1 - step(PRECISION ,col.z));
    FragColor = vec4(col.xyz ,1- a);
    //FragColor = vec4(1.0 ,0,0, 1.0);
}