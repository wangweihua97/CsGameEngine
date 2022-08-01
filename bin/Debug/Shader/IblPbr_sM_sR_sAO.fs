#version 430 core
out vec4 FragColor;
in comVout
{	vec3 WorldPos;
    vec2 UV;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
}fIn;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D emissiveMap;
uniform float _EmissiveIntensity;
// lights
#include Common/Shadow.glsl
#include Common/CommonCore.glsl
#include Common/IblPbr.glsl

void main()
{	
    vec3 albedo     = pow(texture(albedoMap, fIn.UV).rgb, vec3(2.2));
    float metallic  = texture(metallicMap, fIn.UV).r;
    float roughness = texture(roughnessMap, fIn.UV).r;
    float ao        = texture(aoMap, fIn.UV).r;
    vec3 emissive   = texture(emissiveMap, fIn.UV).rgb;

    vec3 N = getNormalFromMap();
    //mat3 TBN = mat3(fIn.Tangent, fIn.Bitangent, fIn.Normal);
	//vec3 N = getNormalFromMap(normalMap ,fIn.UV ,TBN);
    vec3 V = normalize(viewPos - fIn.WorldPos);
    vec3 L = normalize(-lightDir.xyz);
    float attenuation = 1 - ShadowFactor(fIn.WorldPos ,N ,L);
    vec3 LColor = lightcolor.xyz * ambient_intensity.w;
    vec4 final = GetIblPbr(albedo,metallic,roughness,ao, N,V,L ,attenuation , LColor);
    final += _EmissiveIntensity * 5 * vec4(emissive ,1);
    FragColor = final;
    //FragColor = vec4(albedo ,1);
}