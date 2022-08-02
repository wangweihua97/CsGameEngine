#version 430 core
out vec4 FragColor;
in vec3 WorldPos;
in vec2 TexCoord;

uniform sampler2D albedoMap;
uniform samplerCube environmentMap;
uniform samplerCube prefilterMap;

void main()
{		
    vec3 envColor = texture(environmentMap, normalize(WorldPos)).rgb;
    vec3 envColor2 = texture(prefilterMap, normalize(WorldPos)).rgb;
    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
    
    FragColor = vec4(envColor + envColor2, 1.0);
}

