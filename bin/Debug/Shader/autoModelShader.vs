#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

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
void main()
{

	// Transformed vertex position 
    TexCoords = aTexCoords;
    WorldPos = (model * vec4(aPos, 1.0)).xyz;
    Normal = mat3(model) * aNormal;   

    //gl_Position =  projection * view * vec4(WorldPos, 1.0);
    gl_Position =  projection * view * model * vec4(aPos, 1.0);
}