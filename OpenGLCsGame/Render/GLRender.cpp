#include "GLRender.h"
#include "Material.h"
#include "../GameObject/GameObject.h"
#include "../Component/RenderComponent.h"
#include "../Component/ModelComponent.h"
#include "../Component/CameraComponent.h"
#include "../Camera/Camera.h"
#include "../Scene/Scene.h"

GLRender::GLRender()
{
    
}
// ------------------------------------------------------------------------
GLRender::~GLRender()
{
    delete m_CommandBuffer;

    delete m_NDCPlane;

    delete m_GBuffer;
    delete m_CustomTarget;

    // shadows
    for (int i = 0; i < m_ShadowRenderTargets.size(); ++i)
    {
        delete m_ShadowRenderTargets[i];
    }

    // post-processing
    delete m_PostProcessTarget;

    // pbr
    //delete m_PBR;
}
// ------------------------------------------------------------------------
void GLRender::Init(GLADloadproc loadProcFunc)
{
    // initialize render items
    m_CommandBuffer = new RenderCommandQueue(this);
    m_CustomTarget = new RenderTarget(m_RenderSize.x, m_RenderSize.y, GL_FLOAT, 1, true);
    m_PostProcessTarget = new RenderTarget(m_RenderSize.x, m_RenderSize.y, GL_UNSIGNED_BYTE, 1, false);
    m_Camera = new Camera();
    m_PostProcessor = new PostProcess(this);
    // configure default OpenGL state
    m_GLCache.SetDepthTest(true);
    m_GLCache.SetCull(true);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // shadows
    for (int i = 0; i < 1; ++i) // allow up to a total of 4 dir/spot shadow casters
    {
        RenderTarget* rt = new RenderTarget(2048, 2048, GL_UNSIGNED_BYTE, 1, true);
        rt->GetDepthStencilTexture()->Bind();
        rt->GetDepthStencilTexture()->FilterMin = GL_NEAREST;
        rt->GetDepthStencilTexture()->FilterMax = GL_NEAREST;
        rt->GetDepthStencilTexture()->SetWrapMode(GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        m_ShadowRenderTargets.push_back(rt);
    }
    glViewport(0.0f, 0.0f, m_RenderSize.x, m_RenderSize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    m_NDCPlane = new Quad;
    return;

    
    glGenFramebuffers(1, &m_FramebufferCubemap);
    glGenRenderbuffers(1, &m_CubemapDepthRBO);

    

    


    // ubo
    glGenBuffers(1, &m_GlobalUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_GlobalUBO);
    glBufferData(GL_UNIFORM_BUFFER, 720, nullptr, GL_STREAM_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_GlobalUBO);
}
// ------------------------------------------------------------------------
void GLRender::SetRenderSize(unsigned int width, unsigned int height)
{
    m_RenderSize.x = width;
    m_RenderSize.y = height;

    //m_GBuffer->Resize(width, height);
    if(m_CustomTarget)
        m_CustomTarget->Resize(width, height);
    if (m_PostProcessTarget)
        m_PostProcessTarget->Resize(width, height);
}
// ------------------------------------------------------------------------
glm::vec2 GLRender::GetRenderSize()
{
    return m_RenderSize;
}
// ------------------------------------------------------------------------
void GLRender::SetTarget(RenderTarget* renderTarget, GLenum target)
{
    m_CurrentRenderTargetCustom = renderTarget;
    if (renderTarget != nullptr)
    {
        if (std::find(m_RenderTargetsCustom.begin(), m_RenderTargetsCustom.end(), renderTarget)
            == m_RenderTargetsCustom.end())
        {
            m_RenderTargetsCustom.push_back(renderTarget);
        }
    }
}
// ------------------------------------------------------------------------
Camera* GLRender::GetCamera()
{
    return m_Camera;
}
// ------------------------------------------------------------------------
void GLRender::SetCamera(Camera* camera)
{
    m_Camera = camera;
}
// ------------------------------------------------------------------------
void GLRender::PushRender(Mesh* mesh, Material* material, glm::mat4 transform, glm::mat4 prevFrameTransform , boost::function<void()> beforeRender)
{
    // get current render target
    RenderTarget* target = getCurrentRenderTarget();
    // don't render right away but push to the command buffer for later rendering.
    m_CommandBuffer->Push(mesh, material, transform, glm::vec3(-99999.0f), glm::vec3(99999.0f), target , beforeRender);
}
void GLRender::PushShadowRender(Mesh* mesh, Material* material, glm::mat4 transform, glm::mat4 prevFrameTransform, boost::function<void()> beforeRender)
{
    m_CommandBuffer->PushShadow(mesh, material, transform, glm::vec3(-99999.0f), glm::vec3(99999.0f), beforeRender);
}
// ------------------------------------------------------------------------
void GLRender::PushRender(GameObject* node)
{
    // get current render target
    RenderTarget* target = getCurrentRenderTarget();
    // traverse through all the scene nodes and for each node: push its render state to the 
    // command buffer together with a calculated transform matrix.
    auto renderComponent = node->GetComponent<RenderComponent>();
    auto modelComponent = node->GetComponent<ModelComponent>();
    if (renderComponent && modelComponent)
    {
        for (auto mesh : modelComponent->meshes)
        {
            m_CommandBuffer->Push(mesh, &(renderComponent->material), node->transform->worldTransformMat, glm::vec3(1.0f), glm::vec3(1.0f), target);
        }
    }
}
// ------------------------------------------------------------------------
void GLRender::PushPostProcessor(Material* postProcessor)
{
    // we only care about the material, mesh as NDC quad is pre-defined.
    //m_CommandBuffer->Push(nullptr, postProcessor);
}
// ------------------------------------------------------------------------
void GLRender::AddLight(DirectionalLight* light)
{
    m_DirectionalLights.push_back(light);
}
// ------------------------------------------------------------------------
void GLRender::RenderPushedCommands()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // sort all pushed render commands by heavy state-switches e.g. shader switches.
    m_CommandBuffer->Sort();

    // update (global) uniform buffers
    updateGlobalUBOs();

    // set default GL state
    m_GLCache.SetBlend(false);
    m_GLCache.SetCull(true);
    m_GLCache.SetCullFace(GL_BACK);
    m_GLCache.SetDepthTest(true);
    m_GLCache.SetDepthFunc(GL_LEQUAL);

    // 2. render all shadow casters to light shadow buffers
    //if (Shadows)
    if (Shadows)
    {
        m_GLCache.SetCullFace(GL_FRONT);
        std::vector<RenderCommand> shadowRenderCommands = m_CommandBuffer->GetShadowCastRenderCommands();
        m_ShadowViewProjections.clear();

        unsigned int shadowRtIndex = 0;
        for (int i = 0; i < m_DirectionalLights.size(); ++i)
        {
            DirectionalLight* light = m_DirectionalLights[i];
            if (light->CastShadows)
            {
                //m_MaterialLibrary->dirShadowShader->Use();

                glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowRenderTargets[shadowRtIndex]->ID);
                glViewport(0, 0, m_ShadowRenderTargets[shadowRtIndex]->Width, m_ShadowRenderTargets[shadowRtIndex]->Height);
                glClear(GL_DEPTH_BUFFER_BIT);

                glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
                glm::mat4 lightView = glm::lookAt(-Scene::Instace->light.Direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0,1.0f,0.0));

                m_DirectionalLights[i]->LightSpaceViewProjection = lightProjection * lightView;
                m_DirectionalLights[i]->ShadowMapRT = m_ShadowRenderTargets[shadowRtIndex];
                for (int j = 0; j < shadowRenderCommands.size(); ++j)
                {
                    renderShadowCastCommand(&shadowRenderCommands[j] , lightProjection , lightView);
                }
                ++shadowRtIndex;
            }
        }
        m_GLCache.SetCullFace(GL_BACK);
    }

    for (unsigned int targetIndex = 0; targetIndex < m_RenderTargetsCustom.size(); ++targetIndex)
    {
        RenderTarget* renderTarget = m_RenderTargetsCustom[targetIndex];
        if (renderTarget)
        {
            glViewport(0, 0, renderTarget->Width, renderTarget->Height);
            glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->ID);
            if (renderTarget->HasDepthAndStencil)
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            else
                glClear(GL_COLOR_BUFFER_BIT);
            m_Camera->SetPerspective(m_Camera->FOV,
                (float)renderTarget->Width / (float)renderTarget->Height,
                0.1, 100.0f);
        }
        else
        {
            // don't render to default framebuffer, but to custom target framebuffer which 
            // we'll use for post-processing.
            glViewport(0, 0, m_RenderSize.x, m_RenderSize.y);
            glBindFramebuffer(GL_FRAMEBUFFER, m_CustomTarget->ID);
            m_Camera->SetPerspective(m_Camera->FOV, m_RenderSize.x / m_RenderSize.y, 0.1,
                100.0f);
        }

        // sort all render commands and retrieve the sorted array
        std::vector<RenderCommand> renderCommands = m_CommandBuffer->GetCustomRenderCommands(renderTarget);

        // terate over all the render commands and execute
        m_GLCache.SetPolygonMode(Wireframe ? GL_LINE : GL_FILL);
        for (unsigned int i = 0; i < renderCommands.size(); ++i)
        {
            renderCustomCommand(&renderCommands[i], nullptr);
        }
        m_GLCache.SetPolygonMode(GL_FILL);
    }

    // alpha material pass
    /*glViewport(0, 0, m_RenderSize.x, m_RenderSize.y);
    glBindFramebuffer(GL_FRAMEBUFFER, m_CustomTarget->ID);
    std::vector<RenderCommand> alphaRenderCommands = m_CommandBuffer->GetAlphaRenderCommands(true);
    for (unsigned int i = 0; i < alphaRenderCommands.size(); ++i)
    {
        renderCustomCommand(&alphaRenderCommands[i], nullptr);
    }

    std::vector<RenderCommand> postProcessingCommands = m_CommandBuffer->GetPostProcessingRenderCommands();
    for (unsigned int i = 0; i < postProcessingCommands.size(); ++i)
    {
        // ping-pong between render textures
        bool even = i % 2 == 0;
        Blit(even ? m_CustomTarget->GetColorTexture(0) : m_PostProcessTarget1->GetColorTexture(0),
            even ? m_PostProcessTarget1 : m_CustomTarget,
            postProcessingCommands[i].Material);
    }*/

    // clear the command buffer s.t. the next frame/call can start from an empty slate again.
    m_CommandBuffer->Clear();

    // clear render state
    m_RenderTargetsCustom.clear();
    m_CurrentRenderTargetCustom = nullptr;
}
// ------------------------------------------------------------------------
void GLRender::Blit(Texture2D* src,
    RenderTarget* dst,
    Material* material,
    std::string   textureUniformName)
{
    // if a destination target is given, bind to its framebuffer
    if (dst)
    {
        glViewport(0, 0, dst->Width, dst->Height);
        glBindFramebuffer(GL_FRAMEBUFFER, dst->ID);
        if (dst->HasDepthAndStencil)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        else
            glClear(GL_COLOR_BUFFER_BIT);
    }
    // else we bind to the default framebuffer
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_RenderSize.x, m_RenderSize.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    // if no material is given, use default blit material
    if (!material)
    {
        //material = m_MaterialLibrary->defaultBlitMaterial;
    }
    // if a source render target is given, use its color buffer as input to material shader.
    if (src)
    {
        material->SetTexture(textureUniformName, src);
    }
    // render screen-space material to quad which will be displayed in dst's buffers.
    RenderCommand command;
    command.Material = material;
    command.Mesh = m_NDCPlane;
    renderCustomCommand(&command, nullptr);
}
// ------------------------------------------------------------------------
void GLRender::renderCustomCommand(RenderCommand* command, Camera* customCamera, bool updateGLSettings)
{
    Material* material = command->Material;
    Mesh* mesh = command->Mesh;

    // update global GL blend state based on material
    if (updateGLSettings)
    {
        m_GLCache.SetBlend(material->Blend);
        if (material->Blend)
        {
            m_GLCache.SetBlendFunc(material->BlendSrc, material->BlendDst);
        }
        m_GLCache.SetDepthFunc(material->DepthCompare);
        m_GLCache.SetDepthWrite(material->DepthWrite);
        m_GLCache.SetDepthTest(material->DepthTest);
        m_GLCache.SetCull(material->Cull);
        m_GLCache.SetCullFace(material->CullFace);
    }

    // default uniforms that are always configured regardless of shader configuration (see them 
    // as a default set of shader uniform variables always there); with UBO
    material->GetShader()->Use();
    material->GetShader()->SetMatrix4("model", command->Transform);
    
    if (customCamera) // pass custom camera specific uniform
    {
        material->GetShader()->SetMatrix4("projection", customCamera->Projection);
        material->GetShader()->SetMatrix4("view", customCamera->View);
        material->GetShader()->SetVector3f("CamPos", customCamera->Position);
    }
    if (!command->BeforeRender.empty())
        command->BeforeRender();
    if (mesh == 0)
        return;
    
    //material->GetShader()->SetMatrix4("prevModel", command->PrevTransform);

    //material->GetShader()->SetInteger("ShadowsEnabled", Shadows);

    /*if (Shadows && material->Type == MATERIAL_CUSTOM && material->ShadowReceive)
    {
        for (int i = 0; i < m_DirectionalLights.size(); ++i)
        {
            if (m_DirectionalLights[i]->ShadowMapRT)
            {
                material->GetShader()->SetMatrix4(("lightShadowViewProjection" + std::to_string(i + 1)).c_str(), m_DirectionalLights[i]->LightSpaceViewProjection);
                m_DirectionalLights[i]->ShadowMapRT->GetDepthStencilTexture()->Bind(10 + i);
            }
        }
    }*/
    if (material->ShadowReceive)
    {
        material->GetShader()->SetInteger("lightShadowMap", 10);
        material->GetShader()->SetMatrix4("lightShadowViewProjection", m_DirectionalLights[0]->LightSpaceViewProjection);
        m_DirectionalLights[0]->ShadowMapRT->GetDepthStencilTexture()->Bind(10);
    }

    if (material->usePbr)
    {
        material->GetShader()->SetInteger("irradianceMap", 15);
        glActiveTexture(GL_TEXTURE15);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Scene::Instace->ibl.irradianceMap);
        material->GetShader()->SetInteger("prefilterMap", 16);
        glActiveTexture(GL_TEXTURE16);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Scene::Instace->ibl.prefilterMap);
        material->GetShader()->SetInteger("brdfLUT", 17);
        glActiveTexture(GL_TEXTURE17);
        glBindTexture(GL_TEXTURE_2D, Scene::Instace->ibl.brdfLUTTexture);
    }

    // bind/active uniform sampler/texture objects
    auto* samplers = material->GetShaderTextures();
    for (auto it = samplers->begin(); it != samplers->end(); ++it)
    {
        if (it->second.Type == SHADER_TYPE_SAMPLER2D)
        {
            it->second.TextureWithBind.Texture->Bind(it->second.TextureWithBind.bindPos);
            glUniform1i(glGetUniformLocation(material->GetShader()->ID, it->first.c_str()), it->second.TextureWithBind.bindPos);
        }
        else
        {
            it->second.TextureCube.Texture->Bind(it->second.TextureCube.bindPos);
            glUniform1i(glGetUniformLocation(material->GetShader()->ID, it->first.c_str()), it->second.TextureCube.bindPos);
        }
        
    }

    // set uniform state of material
    auto* uniforms = material->GetShaderValues();
    for (auto it = uniforms->begin(); it != uniforms->end(); ++it)
    {
        switch (it->second.Type)
        {
        case SHADER_TYPE_BOOL:
            material->GetShader()->SetInteger(it->first.c_str(), it->second.Bool);
            break;
        case SHADER_TYPE_INT:
            material->GetShader()->SetInteger(it->first.c_str(), it->second.Int);
            break;
        case SHADER_TYPE_FLOAT:
            material->GetShader()->SetFloat(it->first.c_str(), it->second.Float);
            break;
        case SHADER_TYPE_VEC2:
            material->GetShader()->SetVector2f(it->first.c_str(), it->second.Vec2);
            break;
        case SHADER_TYPE_VEC3:
            material->GetShader()->SetVector3f(it->first.c_str(), it->second.Vec3);
            break;
        case SHADER_TYPE_VEC4:
            material->GetShader()->SetVector4f(it->first.c_str(), it->second.Vec4);
            break;
        case SHADER_TYPE_MAT2:
            material->GetShader()->SetMatrix4(it->first.c_str(), it->second.Mat2);
            break;
        case SHADER_TYPE_MAT3:
            material->GetShader()->SetMatrix4(it->first.c_str(), it->second.Mat3);
            break;
        case SHADER_TYPE_MAT4:
            material->GetShader()->SetMatrix4(it->first.c_str(), it->second.Mat4);
            break;
        default:
            std::cout<<"Unrecognized Uniform type set"<< std::endl;
            break;
        }
    }

    renderMesh(mesh);
}
// --------------------------------------------------------------------------------------------
void GLRender::renderMesh(Mesh* mesh)
{
    glBindVertexArray(mesh->VAO);
    if (mesh->indices.size() > 0)
    {
        glDrawElements(mesh->Topology, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(mesh->Topology, 0, mesh->vertices.size());
    }
}
// --------------------------------------------------------------------------------------------
void GLRender::updateGlobalUBOs()
{
    /*glBindBuffer(GL_UNIFORM_BUFFER, m_GlobalUBO);
    // transformation matrices
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &(m_Camera->Projection * m_Camera->View)[0][0]); // sizeof(glm::mat4) = 64 bytes
    glBufferSubData(GL_UNIFORM_BUFFER, 64, sizeof(glm::mat4), &m_PrevViewProjection[0][0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 128, sizeof(glm::mat4), &m_Camera->Projection[0][0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 192, sizeof(glm::mat4), &m_Camera->View[0][0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 256, sizeof(glm::mat4), &m_Camera->View[0][0]); // TODO: make inv function in glm library
    // scene data
    glBufferSubData(GL_UNIFORM_BUFFER, 320, sizeof(glm::vec4), &m_Camera->Position[0]);
    // lighting
    unsigned int stride = 2 * sizeof(glm::vec4);
    for (unsigned int i = 0; i < m_DirectionalLights.size() && i < 4; ++i) // no more than 4 directional lights
    {
        glBufferSubData(GL_UNIFORM_BUFFER, 336 + i * stride, sizeof(glm::vec4), &m_DirectionalLights[i]->Direction[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, 336 + i * stride + sizeof(glm::vec4), sizeof(glm::vec4), &m_DirectionalLights[i]->Color[0]);
    }
    for (unsigned int i = 0; i < m_PointLights.size() && i < 8; ++i) //  constrained to max 8 point lights in forward context
    {
        glBufferSubData(GL_UNIFORM_BUFFER, 464 + i * stride, sizeof(glm::vec4), &m_PointLights[i]->Position[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, 464 + i * stride + sizeof(glm::vec4), sizeof(glm::vec4), &m_PointLights[i]->Color[0]);
    }*/
}
// --------------------------------------------------------------------------------------------
RenderTarget* GLRender::getCurrentRenderTarget()
{
    return m_CurrentRenderTargetCustom;
}

// --------------------------------------------------------------------------------------------
void GLRender::renderShadowCastCommand(RenderCommand* command, const glm::mat4& projection, const glm::mat4& view)
{
    Shader* shadowShader = command->Material->GetShader();
    shadowShader->Use();
    shadowShader->SetInteger("_HavaAnimation", 0, false);
    shadowShader->SetMatrix4("projection", projection);
    shadowShader->SetMatrix4("view", view);
    shadowShader->SetMatrix4("model", command->Transform);

    if (command->BeforeRender != 0)
        command->BeforeRender();

    renderMesh(command->Mesh);
}