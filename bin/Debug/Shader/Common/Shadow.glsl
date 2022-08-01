
uniform sampler2D lightShadowMap;
uniform mat4 lightShadowViewProjection;
float ShadowFactor(vec3 worldPos, vec3 N, vec3 L)
{   
    vec4 fragPosLightSpace = lightShadowViewProjection * vec4(worldPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(lightShadowMap, projCoords.xy).r; 
    // depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    
    // shadow bias
    float bias = max(0.05 * (1.0 - dot(N, L)), 0.001);
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(lightShadowMap, 0);
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(lightShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;          
        }    
    }
    shadow /= 25.0;
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;    
}