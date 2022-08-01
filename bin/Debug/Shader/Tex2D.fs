#version 430 core
in comVout
{	vec3 WorldPos;
    vec2 UV;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
}fIn;
layout(std140 , binding = 0) uniform PV
{
	mat4 projection;
	mat4 view;
};

layout(std140 , binding = 1) uniform BaseLight
{
	vec4 lightDir;
	vec4 lightcolor;
	vec4 ambient_intensity;
};
layout(std140 , binding = 2) uniform BaseView
{
	vec3 viewPos;
};

out vec4 FragColor;
uniform vec4 _MainColor;
uniform sampler2D albedoMap;

#include Common/Shadow.glsl

void main()
{
    vec2 uv = fIn.UV;
    vec3 N = normalize(fIn.Normal);
    vec3 V = normalize(viewPos - fIn.WorldPos);
    vec3 L = normalize(-lightDir.xyz);
    vec3 H = normalize(V + L);
    float atten = 1 - ShadowFactor(fIn.WorldPos ,N ,L);
    vec3 mainColor = pow(texture(albedoMap, fIn.UV).rgb, vec3(2.2));
    vec3 diffuseColor = mainColor * _MainColor.xyz;
    float diff = max(atten * dot(L, N), ambient_intensity.x);
    vec3 diffuse = ambient_intensity.w * lightcolor.xyz * diff * ambient_intensity.w * diffuseColor;

    float spec = atten * pow(max(dot(N, H), 0.0),32);
    vec3 specular = ambient_intensity.w * lightcolor.xyz * spec * _MainColor.xyz * mainColor;
    //vec3 specular = ambient_intensity.w * lightcolor.xyz * spec * c;
    vec3 color = diffuse + specular;
    color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0/2.2)); 
    FragColor = vec4(color, 1.0);
    //FragColor = vec4(1,1,1, 1.0);
}