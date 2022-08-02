#version 430 core
struct comVout
{	vec3 WorldPos;
    vec2 UV;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
};
layout(std140 , binding = 0) uniform PV
{
	mat4 projection;
	mat4 view;
};

layout(std140 , binding = 1) uniform BaseLight
{
	vec4 lightDir;
	vec4 lightcolor;
	vec4 ambient;
};
layout(std140 , binding = 2) uniform BaseView
{
	vec3 viewPos;
};

vec3 getNormalFromMap(sampler2D normalMap ,vec2 TexCoords ,mat3 TBN)
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;
    return normalize(TBN * tangentNormal);
};
in comVout fIn;
out vec4 FragColor;

uniform sampler2D albedoMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
void main()
{
    vec2 uv = fIn.UV;
    mat3 TBN = mat3(fIn.Tangent, fIn.Bitangent, fIn.Normal);
	vec3 N = getNormalFromMap(normalMap ,fIn.UV ,TBN);
    vec3 V = normalize(viewPos - fIn.WorldPos);
    vec3 L = normalize(-lightDir.xyz);
    vec3 H = normalize(V + L);

    vec3 diffuseColor = texture(albedoMap, uv).rgb;
    float diff = max(dot(L, N), 0.0);
    vec3 diffuse = lightcolor.xyz * diff * diffuseColor;

    float spec = pow(max(dot(N, H), 0.0),8);
    vec3 specular = lightcolor.xyz * spec * texture(specularMap, uv).rgb;
    FragColor = vec4(diffuse + specular, 1.0);
}