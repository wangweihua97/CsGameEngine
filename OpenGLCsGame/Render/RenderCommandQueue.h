#ifndef RenderCommandQueue_H
#define RenderCommandQueue_H
class GLRender;
class Mesh;
class Material;
class RenderTarget;
#include <vector>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <boost/bind/bind.hpp>
#include <boost/function.hpp>

struct RenderCommand
{
    glm::mat4 Transform;
    Mesh* Mesh;
    Material* Material;
    glm::vec3 BoxMin;
    glm::vec3 BoxMax;
    boost::function<void()> BeforeRender;
};
class RenderCommandQueue
{
public:

private:
    GLRender* m_Renderer;

    std::vector<RenderCommand> m_AlphaRenderCommands;
    std::vector<RenderCommand> m_PostProcessingRenderCommands;
    std::vector<RenderCommand> m_ShadowCastRenderCommands;
    std::map<RenderTarget*, std::vector<RenderCommand>> m_CustomRenderCommands;


public:
    RenderCommandQueue(GLRender* renderer);
    ~RenderCommandQueue();

    // pushes render state relevant to a single render call to the command buffer.
    void Push(Mesh* mesh, Material* material, glm::mat4 transform, glm::vec3 boxMin, glm::vec3 boxMax , RenderTarget* target = nullptr , boost::function<void()> beforeRender = 0);
    void PushShadow(Mesh* mesh, Material* material, glm::mat4 transform, glm::vec3 boxMin, glm::vec3 boxMax, boost::function<void()> beforeRender = 0);

    void Clear();
    void Sort();

    std::vector<RenderCommand> GetAlphaRenderCommands(bool cull = false);

    std::vector<RenderCommand> GetCustomRenderCommands(RenderTarget* target, bool cull = false);

    std::vector<RenderCommand> GetPostProcessingRenderCommands();
    std::vector<RenderCommand> GetShadowCastRenderCommands();
};
#endif

