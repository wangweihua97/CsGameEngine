#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

#include Common/CommonCore.glsl

out vec3 WorldPos;
out vec2 TexCoord;
uniform mat4 model;

void main()
{
    WorldPos = aPos;
	mat4 rotView = mat4(mat3(view));
	rotView[3][3] = 1.0;
	vec4 clipPos = projection * rotView * vec4(WorldPos, 1.0);
	gl_Position = clipPos.xyww;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}