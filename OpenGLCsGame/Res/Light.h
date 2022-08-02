#ifndef Light_H
#define Light_H
#include "../Render/RenderTarget.h"

struct DirectionalLightFromCS
{
public:
    glm::vec3 Direction;
    glm::vec3 Color;
    float Intensity;
    bool CastShadows;
};

class DirectionalLight
{
public:
    glm::vec3 Direction = glm::vec3(0.0f);
    glm::vec3 Color = glm::vec3(1.0f);
    float Intensity = 1.0f;
    bool CastShadows = true;
    RenderTarget* ShadowMapRT;
    glm::mat4    LightSpaceViewProjection;
public:
    void SetData(DirectionalLightFromCS data)
    {
        Direction = data.Direction;
        Color = data.Color;
        Intensity = data.Intensity;
        CastShadows = data.CastShadows;
    }

    DirectionalLightFromCS GetData()
    {
        DirectionalLightFromCS data;
        data.Direction = Direction;
        data.Color = Color;
        data.Intensity = Intensity;
        data.CastShadows = CastShadows;
        return data;
    }
};


#endif

