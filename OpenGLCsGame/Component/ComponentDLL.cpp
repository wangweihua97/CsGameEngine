#include "ComponentDLL.h"
#include "CubeMeshComponent.h"
#include "../Res/ResourceManager.h"
#include "ModelComponent.h"
#include "CsComponent.h"
#include "../GameObject/GameObject.h"

void ModelComponentLoadModel(ModelComponent* model, string modelPath, bool gamma)
{
	model->LoadModel(modelPath, false);
}

void ModelComponentInitAnimation(ModelComponent* model)
{
	model->InitAnimation();
}

CsComponent* AddCsComponent(GameObject* go)
{
	return go->AddComponent<CsComponent>();
}

CubeMeshComponent* AddCubeMeshComponent(GameObject* go)
{
	return go->AddComponent<CubeMeshComponent>();
}

void ModelComponent_SetAnimationSpeed(ModelComponent* modelComponent, float s)
{
	;
}

int ModelComponent_GetAnimationCount(ModelComponent* modelComponent)
{
	return modelComponent->animations.size();
}

const char* ModelComponent_GetAnimationName(ModelComponent* modelComponent, int index)
{
	return modelComponent->animations.right.find(index)->second.c_str();
}

void ModelComponent_PlayAnimation(ModelComponent* modelComponent, char* name)
{
	return modelComponent->PlayAnimation(name);
}
