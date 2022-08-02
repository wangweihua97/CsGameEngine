#ifndef ComponentDLL_H
#define ComponentDLL_H

#ifndef DLLFUNC
#define DLLFUNC(return_type) extern "C" __declspec(dllexport) return_type __cdecl
#endif
#include<iostream>
#include<glm/glm.hpp>
using namespace std;

class CubeMeshComponent;
class ModelComponent;
class CsComponent;
class GameObject;
DLLFUNC(void) ModelComponentLoadModel(ModelComponent* model, string modelPath ,bool gamma);
DLLFUNC(void) ModelComponentInitAnimation(ModelComponent* model);
DLLFUNC(CsComponent*) AddCsComponent(GameObject* go);
DLLFUNC(CubeMeshComponent*) AddCubeMeshComponent(GameObject* go);

DLLFUNC(void) ModelComponent_SetAnimationSpeed(ModelComponent* modelComponent, float s);
DLLFUNC(int) ModelComponent_GetAnimationCount(ModelComponent* modelComponent);
DLLFUNC(const char*) ModelComponent_GetAnimationName(ModelComponent* modelComponent, int index);
DLLFUNC(void) ModelComponent_PlayAnimation(ModelComponent* modelComponent, char* name);
#endif
