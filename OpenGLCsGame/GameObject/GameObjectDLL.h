#ifndef GameObjectDLL_H
#define GameObjectDLL_H

#ifndef DLLFUNC
#define DLLFUNC(return_type) extern "C" __declspec(dllexport) return_type __cdecl
#endif

#include"GameObject.h"
#include<glm/glm.hpp>
class CameraComponent;
class MoveComponent;
class CubeMeshComponent;
class ModelComponent;
class Transform;

DLLFUNC(CameraComponent*) AddCameraComponent(GameObject* go);
DLLFUNC(MoveComponent*) AddMoveComponent(GameObject* go);
DLLFUNC(ModelComponent*) AddModelComponent(GameObject* go);
DLLFUNC(Transform*) GetTransform(GameObject* go);
DLLFUNC(void) SetLocalPositon(Transform* go ,glm::vec3 pos);
DLLFUNC(void) SetLocalRotation(Transform* go ,glm::vec3 pos);
DLLFUNC(void) SetLocalScale(Transform* go ,glm::vec3 pos);
DLLFUNC(void) DestoryGameObject(GameObject* go);
#endif
