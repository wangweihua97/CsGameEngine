#include "CameraComponent.h"
#include "../GameObject/GameObject.h"
#include "../Window.h"
#include "../Scene/Scene.h"

CameraComponent::CameraComponent(GameObject* gameObject):Component(gameObject)
{
}

glm::mat4 CameraComponent::GetPerspective()
{
	return glm::perspective(glm::radians(ZOOM), (float)Window::Instance->Width / (float)Window::Instance->Height, 0.1f, 1000.0f);
}

glm::mat4 CameraComponent::GetViewMatrix()
{
    //return glm::lookAt(gameObject->transform->GetWordPosition(), gameObject->transform->GetWordPosition() + gameObject->transform->GetWordForward() , gameObject->transform->GetWordUp());
	//if(gameObject->transform->localXYZAngle.y)
	return glm::lookAt(gameObject->transform->GetWordPosition(), gameObject->transform->GetWordPosition() + gameObject->transform->GetWordForward() , gameObject->transform->GetWordUp());
	//return glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 CameraComponent::GetLookMatrix()
{
	return glm::lookAt(glm::vec3(0.0f), gameObject->transform->GetWordForward(), gameObject->transform->GetWordUp());
}

void CameraComponent::Active()
{
	Scene::Instace->MainCamera = this;
}

void CameraComponent::Update()
{
	__super::Update();
}

void CameraComponent::LateUpdate()
{
	__super::LateUpdate();
}

void CameraComponent::Render()
{
	__super::Render();
}
