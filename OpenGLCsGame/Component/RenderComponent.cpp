#include "RenderComponent.h"
#include "../Window.h"
#include "ModelComponent.h"

RenderComponent::RenderComponent() :Component()
{
}
RenderComponent::RenderComponent(GameObject* gameObject) :Component(gameObject)
{
}

void RenderComponent::SetMaterial(Material& material)
{
	this->material = material;
}

void RenderComponent::Update()
{
}

void RenderComponent::LateUpdate()
{
}

void RenderComponent::Render()
{
	ModelComponent* modelComponent = gameObject->GetComponent<ModelComponent>();
	for (int i = 0; i < modelComponent->meshes.size(); i++)
	{
		Window::Instance->m_render.PushRender(modelComponent->meshes[i], &material, gameObject->transform->worldTransformMat);
	}
	
}
