#ifndef GLRender_H
#define GLRender_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "RenderCommandQueue.h"
#include "GLState.h"
#include "../Res/Light.h"
#include "../Mesh/Quad.h"
#include "../GameObject/GameObject.h"
#include "../Render/RenderTarget.h"
#include "../PostProcess/PostProcess.h"
class Camera;
class GLRender
{
public:
    // configuration
    bool IrradianceGI = true;
    bool Shadows = true;
    bool Lights = true;
    bool RenderLights = true;
    bool LightVolumes = false;
    bool RenderProbes = false;
    bool Wireframe = false;
    RenderTarget* m_CustomTarget;
    RenderTarget* m_PostProcessTarget;
    // shadow buffers
    std::vector<RenderTarget*> m_ShadowRenderTargets;
    std::vector<glm::mat4>    m_ShadowViewProjections;
    PostProcess* m_PostProcessor;
    Quad* m_NDCPlane;
private:
    // render state
    RenderCommandQueue* m_CommandBuffer;
    GLState        m_GLCache;
    glm::vec2     m_RenderSize;

    // lighting
    std::vector<DirectionalLight*> m_DirectionalLights;
    RenderTarget* m_GBuffer = nullptr;
    Mesh* m_DeferredPointMesh;
    // camera
    Camera* m_Camera;

    // render-targets/post
    std::vector<RenderTarget*> m_RenderTargetsCustom;
    RenderTarget* m_CurrentRenderTargetCustom = nullptr;
    
    unsigned int m_FramebufferCubemap;
    unsigned int m_CubemapDepthRBO;

    

    // ubo
    unsigned int m_GlobalUBO;


public:
    GLRender();
    ~GLRender();

    void Init(GLADloadproc loadProcFunc);

    void SetRenderSize(unsigned int width, unsigned int height);
    glm::vec2 GetRenderSize();

    void SetTarget(RenderTarget* renderTarget, GLenum target = GL_TEXTURE_2D);

    Camera* GetCamera();
    void    SetCamera(Camera* camera);

    void PushRender(Mesh* mesh, Material* material, glm::mat4 transform = glm::mat4(), glm::mat4 prevFrameTransform = glm::mat4() , boost::function<void()> beforeRender = 0);
    void PushShadowRender(Mesh* mesh, Material* material, glm::mat4 transform = glm::mat4(), glm::mat4 prevFrameTransform = glm::mat4(), boost::function<void()> beforeRender = 0);
    void PushRender(GameObject* node);
    void PushPostProcessor(Material* postProcessor);

    void AddLight(DirectionalLight* light);

    void RenderPushedCommands();

    void Blit(Texture2D* src, RenderTarget* dst = nullptr, Material* material = nullptr, std::string textureUniformName = "TexSrc");
    void renderMesh(Mesh* mesh);

private:
    // renderer-specific logic for rendering a custom (forward-pass) command
    void renderCustomCommand(RenderCommand* command, Camera* customCamera, bool updateGLSettings = true);
    // updates the global uniform buffer objects
    void updateGlobalUBOs();
    // returns the currently active render target
    RenderTarget* getCurrentRenderTarget();

    // render mesh for shadow buffer generation
    void renderShadowCastCommand(RenderCommand* command , const glm::mat4& projection, const glm::mat4& view);
};
#endif

