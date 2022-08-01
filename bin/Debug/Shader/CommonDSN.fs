#version 430 core

in comVout
{	vec3 WorldPos;
    vec2 UV;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
}fIn;

uniform vec4 _MainColor;
uniform sampler2D albedoMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
out vec4 FragColor;

#include Common/CommonCore.glsl
#include Common/Shadow.glsl

void main()
{
    vec2 uv = fIn.UV;
    mat3 TBN = mat3(fIn.Tangent, fIn.Bitangent, fIn.Normal);
	vec3 N = getNormalFromMap(normalMap ,fIn.UV ,TBN);
    vec3 V = normalize(viewPos - fIn.WorldPos);
    vec3 L = normalize(-lightDir.xyz);
    vec3 H = normalize(V + L);

    float atten = 1 - ShadowFactor(fIn.WorldPos ,N ,L);

    vec3 diffuseColor = pow(texture(albedoMap, fIn.UV).rgb, vec3(2.2));
    float diff = max(atten * dot(L, N), ambient_intensity.x);
    vec3 diffuse = lightcolor.xyz * ambient_intensity.w * diff * diffuseColor;

    float spec = atten * pow(max(dot(N, H), 0.0),32);
    vec3 specular = lightcolor.xyz * spec * pow(texture(specularMap, uv).rgb, vec3(2.2));
    vec3 color = diffuse + specular;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
    FragColor = vec4(color, 1.0);
}