#include "RenderCommandQueue.h"
#include "Material.h"
#include <algorithm>
RenderCommandQueue::RenderCommandQueue(GLRender* renderer)
{
    m_Renderer = renderer;
}
// --------------------------------------------------------------------------------------------
RenderCommandQueue::~RenderCommandQueue()
{
    Clear();
}
// --------------------------------------------------------------------------------------------
void RenderCommandQueue::Push(Mesh* mesh, Material* material, glm::mat4 transform, glm::vec3 boxMin, glm::vec3 boxMax, RenderTarget* target, boost::function<void()> beforeRender)
{
    RenderCommand command = {};
    command.Mesh = mesh;
    command.Material = material;
    command.Transform = transform;
    command.BoxMin = boxMin;
    command.BoxMax = boxMax;
    command.BeforeRender = beforeRender;

    // if material requires alpha support, add it to alpha render commands for later rendering.
    if (material->Blend)
    {
        material->Type = MATERIAL_CUSTOM;
        m_AlphaRenderCommands.push_back(command);
    }
    else
    {
        if (material->Type == MATERIAL_CUSTOM)
        {
            // check if this render target has been pushed before, if so add to vector, 
            // otherwise create new vector with this render target.
            if (m_CustomRenderCommands.find(target) != m_CustomRenderCommands.end())
                m_CustomRenderCommands[target].push_back(command);
            else
            {
                m_CustomRenderCommands[target] = std::vector<RenderCommand>();
                m_CustomRenderCommands[target].push_back(command);
            }
        }
        else if (material->Type == MATERIAL_POST_PROCESS)
        {
            m_PostProcessingRenderCommands.push_back(command);
        }
    }
}
void RenderCommandQueue::PushShadow(Mesh* mesh, Material* material, glm::mat4 transform, glm::vec3 boxMin, glm::vec3 boxMax, boost::function<void()> beforeRender)
{
    RenderCommand command = {};
    command.Mesh = mesh;
    command.Material = material;
    command.Transform = transform;
    command.BoxMin = boxMin;
    command.BoxMax = boxMax;
    command.BeforeRender = beforeRender;

    m_ShadowCastRenderCommands.push_back(command);
}
// --------------------------------------------------------------------------------------------
void RenderCommandQueue::Clear()
{
    m_CustomRenderCommands.clear();
    m_PostProcessingRenderCommands.clear();
    m_AlphaRenderCommands.clear();
    m_ShadowCastRenderCommands.clear();
}

bool renderSortCustom(const RenderCommand& a, const RenderCommand& b)
{
    /*

      We want strict weak ordering, which states that if two objects x and y are equivalent
      then both f(x,y) and f(y,x) should be false. Thus when comparing the object to itself
      the comparison should always equal false.

      We also want to do multiple sort comparisons in a single pass, so we encapsulate all
      relevant properties inside an n-tuple with n being equal to the number of sort queries
      we want to do. The tuple < comparison operator initially compares its left-most element
      and then works along the next elements of the tuple until an outcome is clear.

      Another (non C++11) alternative is to write out both the < and > case with the == case
      defaulting to false as in:

      if(a1 < b1)
        return true;
      if(b1 > a1)
        return false;

      if(a2 < b2)
        return true;
      if(b2 > a2)
        return false;
      [...] and so on for each query you want to perform
      return false;

    */
    return std::make_tuple(a.Material->Blend, a.Material->GetShader()->ID) <
        std::make_tuple(b.Material->Blend, b.Material->GetShader()->ID);
}
bool renderSortShader(const RenderCommand& a, const RenderCommand& b)
{
    return a.Material->GetShader()->ID < b.Material->GetShader()->ID;
}
// --------------------------------------------------------------------------------------------
void RenderCommandQueue::Sort()
{
    for (auto rtIt = m_CustomRenderCommands.begin(); rtIt != m_CustomRenderCommands.end(); rtIt++)
    {
        std::sort(rtIt->second.begin(), rtIt->second.end(), renderSortCustom);
    }
}
// --------------------------------------------------------------------------------------------
std::vector<RenderCommand> RenderCommandQueue::GetCustomRenderCommands(RenderTarget* target, bool cull)
{
    // only cull when on main/null render target
    if (target == nullptr && cull)
    {
        std::vector<RenderCommand> commands;
        for (auto it = m_CustomRenderCommands[target].begin(); it != m_CustomRenderCommands[target].end(); ++it)
        {
            RenderCommand command = *it;
            //if (m_Renderer->GetCamera()->Frustum.Intersect(command.BoxMin, command.BoxMax)) {
                //commands.push_back(command);
            //}
            commands.push_back(command);
        }
        return commands;
    }
    else
    {
        return m_CustomRenderCommands[target];
    }
}
// --------------------------------------------------------------------------------------------
std::vector<RenderCommand> RenderCommandQueue::GetAlphaRenderCommands(bool cull)
{
    if (cull)
    {
        std::vector<RenderCommand> commands;
        for (auto it = m_AlphaRenderCommands.begin(); it != m_AlphaRenderCommands.end(); ++it)
        {
            RenderCommand command = *it;
            //if (m_Renderer->GetCamera()->Frustum.Intersect(command.BoxMin, command.BoxMax)) {
            //    commands.push_back(command);
            //}
            commands.push_back(command);
        }
        return commands;
    }
    else
    {
        return m_AlphaRenderCommands;
    }
}
// --------------------------------------------------------------------------------------------
std::vector<RenderCommand> RenderCommandQueue::GetPostProcessingRenderCommands()
{
    return m_PostProcessingRenderCommands;
}
// --------------------------------------------------------------------------------------------
std::vector<RenderCommand> RenderCommandQueue::GetShadowCastRenderCommands()
{
    return m_ShadowCastRenderCommands;
}