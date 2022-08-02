#include "GameObjectDLL.h"
#include "../Component/CameraComponent.h"
#include "../Component/MoveComponent.h"
#include "../Component/CubeMeshComponent.h"
#include "../Component/ModelComponent.h"

CameraComponent* AddCameraComponent(GameObject* go)
{
	CameraComponent* camera = go->AddComponent<CameraComponent>();
	return camera;
}

MoveComponent* AddMoveComponent(GameObject* go)
{
	MoveComponent* move = go->AddComponent<MoveComponent>();
	return move;
}

ModelComponent* AddModelComponent(GameObject* go)
{
	ModelComponent* model = go->AddComponent<ModelComponent>();
	return model;
}

Transform* GetTransform(GameObject* go)
{
	return go->transform;
}

void SetLocalPositon(Transform* go, glm::vec3 pos)
{
	go->localPosition = pos;
}

void SetLocalRotation(Transform* go, glm::vec3 rotation)
{
	go->localXYZAngle = rotation;
}

void SetLocalScale(Transform* go, glm::vec3 scale)
{
	go->localScale = scale;
}
void DestoryGameObject(GameObject* go)
{
	delete go;
}
