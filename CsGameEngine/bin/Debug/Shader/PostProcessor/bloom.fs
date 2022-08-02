#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D MainTex;
uniform float _LuminanceThreshold;
uniform float _Intensity;

float luminance(vec3 color) {
    //return  0.2125 * color.r + 0.7154 * color.g + 0.0721 * color.b;
    return max(color.r,max(color.g,color.b));
}

void main()
{
    vec3 hdrColor = texture(MainTex, TexCoords).rgb;
    hdrColor = (luminance(hdrColor) - _LuminanceThreshold)*_Intensity*hdrColor;
    hdrColor = clamp(hdrColor ,vec3(0) ,vec3(5));
    FragColor = vec4(hdrColor ,1);
}