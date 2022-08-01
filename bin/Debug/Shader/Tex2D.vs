#version 430 core
out comVout
{	vec3 WorldPos;
    vec2 UV;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
} vOut;
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

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

uniform mat4 model;


void main()
{
    vec4 world = model * vec4(aPos, 1.0);
	gl_Position = projection * view *  world;
	vOut.WorldPos = world.xyz;
	vOut.UV = TexCoords;
	vOut.Normal = Normal;
	vOut.Tangent = Tangent;
	vOut.Bitangent = Bitangent;
}