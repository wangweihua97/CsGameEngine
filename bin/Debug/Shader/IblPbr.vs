#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in ivec4 BoneIDs;
layout (location = 6) in vec4 Weights;

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
uniform mat4 model;

uniform int _HavaAnimation;
const int MAX_BONES = 100; // Max number of bones
uniform mat4 gBones[MAX_BONES]; // Bone transformations 

void main()
{
	vec4 world = model * vec4(aPos, 1.0);
	if(_HavaAnimation >0)
	{
		mat4 BoneTransform = gBones[ BoneIDs[0] ] * Weights[0];
		BoneTransform += gBones[ BoneIDs[1] ] * Weights[1];
		BoneTransform += gBones[ BoneIDs[2] ] * Weights[2];
		BoneTransform += gBones[ BoneIDs[3] ] * Weights[3];
		vec4 tPos = BoneTransform * vec4(aPos, 1.0);
		world = model * tPos;
	}
	gl_Position = projection * view *  world;
	vOut.WorldPos = world.xyz/world.w;
	vOut.UV = aTexCoords;
	vOut.Normal = aNormal;
	vOut.Tangent = aTangent;
	vOut.Bitangent = aBitangent;
}