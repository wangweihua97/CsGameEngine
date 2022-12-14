#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;
layout (location = 5) in ivec4 BoneIDs;
layout (location = 6) in vec4 Weights;

uniform mat4 projection;
uniform mat4 view;
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
	gl_Position =  projection * view * world;
}