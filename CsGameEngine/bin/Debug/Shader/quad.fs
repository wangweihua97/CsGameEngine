#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D MainTex;

void main()
{
    FragColor = vec4(texture(MainTex, TexCoords).rgb ,1.0);
    //FragColor = vec4(0.4824, 0.9922, 0.1451, 1.0);
}