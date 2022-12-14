#include "MoveComponent.h"
#include "../Input.h"
#include "../Time.h"
#include "../GameObject/GameObject.h"
#include "../Window.h"
#include "../EventCenter.h"

MoveComponent::MoveComponent(GameObject* gameObject) :Component(gameObject)
{
}

void MoveComponent::Update()
{
	glm::vec2 moveDir(0.0f);
	moveDir.x = Input::Instance->KeyIsDown(Key::D) ? -1 : Input::Instance->KeyIsDown(Key::A) ? 1 : 0;
	moveDir.y = Input::Instance->KeyIsDown(Key::W) ? 1 : Input::Instance->KeyIsDown(Key::S) ? -1 : 0;
	if(moveDir.x != 0 || moveDir.y != 0)
	    moveDir = glm::normalize(moveDir);

	glm::vec3 position = gameObject->transform->localPosition;
	position += gameObject->transform->GetLocalRight() *  moveDir.x * Time::GetDeltaTime() * moveSpeed;
	position += gameObject->transform->GetLocalForward() * moveDir.y * Time::GetDeltaTime() * moveSpeed;
	gameObject->transform->localPosition = position;

	glm::vec3 rotate = gameObject->transform->localXYZAngle;
	if (Input::Instance->KeyIsDown(Key::Mouse1))
	{
		if (lastMousePos.x < 0)
		{
			lastMousePos = glm::vec2(Input::xpos, Input::ypos);
		}
		else
		{
			glm::vec2 r = glm::vec2(Input::xpos, Input::ypos) - lastMousePos;
			lastMousePos = glm::vec2(Input::xpos, Input::ypos);
			rotate.x += r.y * cameraSense;
			rotate.y -= r.x * cameraSense;
			gameObject->transform->localXYZAngle = rotate;
		}
	}
	else
	{
		if (lastMousePos.x >= 0)
			lastMousePos = glm::vec2(-1);
	}
	//rotate.x = (Input::ypos - (float)Window::Instance->Height/2) / (float)Window::Instance->Height * 179.0f;
	//rotate.y = -(Input::xpos - (float)Window::Instance->Width/2)/ (float)Window::Instance->Width * 359.0f;
	/*rotate.x += Input::yoffset * cameraSense;
	rotate.y -= Input::xoffset * cameraSense;*/
	//gameObject->transform->localXYZAngle = rotate;
	__super::Update();
}

void MoveComponent::LateUpdate()
{
	__super::LateUpdate();
}

void MoveComponent::Render()
{
	__super::Render();
}
