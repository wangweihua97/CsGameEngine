#include "CubeMeshComponent.h"
#include "../Res/ResourceManager.h"
#include "../GameObject/GameObject.h"
#include "../Mesh/Cube.h"
#include "../Window.h"

CubeMeshComponent::CubeMeshComponent(GameObject* gameObject) :Component(gameObject)
{
    mesh = new Cube();
    meshsMat = 0;
}


void CubeMeshComponent::Update()
{
    __super::Update();
}

void CubeMeshComponent::LateUpdate()
{
    __super::LateUpdate();
}

void CubeMeshComponent::Render()
{
    if (meshsMat == nullptr)
        meshsMat = ResourceManager::GetDefalutMat();
    Window::Instance->m_render.PushRender(mesh, meshsMat, gameObject->transform->worldTransformMat);
    __super::Render();
}

void CubeMeshComponent::RenderShadow()
{
    Material* shadowMat = ResourceManager::GetShadowMat();
    Window::Instance->m_render.PushShadowRender(mesh, shadowMat, gameObject->transform->worldTransformMat);
    __super::RenderShadow();
}



void CubeMeshComponent_SetMeshMat(CubeMeshComponent* cube, Material* mat)
{
    cube->meshsMat = mat;
}
