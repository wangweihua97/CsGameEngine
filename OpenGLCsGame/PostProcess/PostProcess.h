#ifndef PostProcess_H
#define PostProcess_H
#include "Bloom.h"
#include "../Render/Shader.h"
class GLRender;
class PostProcess
{
private:
    GLRender* m_renderer;
    Bloom m_bloom;
    GLRender* m_quadShader;
    Shader* Quad;
public:
    float BloomThreshold = 1;
    float BloomIntensity = 1;
    bool UseBloom = false;
    RenderTarget* m_PostProcessTarget;
    PostProcess(GLRender* renderer);
    ~PostProcess();
    void Bloom(RenderTarget* src);
    void DoPostProcess(RenderTarget* src);
    void RenderQuad();
};

#ifndef DLLFUNC
#define DLLFUNC(return_type) extern "C" __declspec(dllexport) return_type __cdecl
#endif

DLLFUNC(void) PostProcess_SetBloomThreshold(float threshold);
DLLFUNC(void) PostProcess_SetBloomIntensity(float intensity);
DLLFUNC(void) PostProcess_SetUseBloom(bool isUse);
#endif

